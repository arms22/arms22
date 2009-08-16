#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

static struct termios g_oldtio;
static int g_dev_port = -1;

int dev_open(const char *port)
{
	struct termios newtio;
	
	g_dev_port = open(port, O_RDWR | O_NOCTTY);
	if(g_dev_port < 0){
		fprintf(stderr, "%s could not be opened.\n", port);
		return -1;
	}
	
	if( tcgetattr(g_dev_port, &g_oldtio) != 0){
		fprintf(stderr, "could not get attr.\n");
		close(g_dev_port);
		return -1;
	}
	
	memset(&newtio, 0, sizeof(newtio));
	
	/* 8bit, no parity, one stop-bit */
	newtio.c_cflag 	   = B9600 | CS8 | CLOCAL | CREAD;
	newtio.c_iflag 	   = 0;
	newtio.c_oflag 	   = 0;
 	newtio.c_lflag 	   = 0;
	newtio.c_cc[VTIME] = 1;
	newtio.c_cc[VMIN]  = 0;
	
	cfsetispeed(&newtio, B9600);
	cfsetospeed(&newtio, B9600);
	
	if( tcsetattr(g_dev_port, TCSAFLUSH, &newtio) != 0){
		fprintf(stderr, "could not set attr.\n");
		close(g_dev_port);
		return -1;
	}
	
	return 0;
}

int dev_close(void)
{
	if(g_dev_port < 0)
		;
	else{
		tcsetattr(g_dev_port, TCSAFLUSH, &g_oldtio);
		close(g_dev_port);
	}
	g_dev_port = -1;
	return 0;
}

void dev_flush(void)
{
	tcflush(g_dev_port,TCIOFLUSH);
}

int dev_putchar(int c)
{
	char p = c;
	if(write(g_dev_port, &p, 1) <= 0)
		return 0;
	return 1;
}

int dev_getchar(void)
{
	unsigned char c;
	do {
		ssize_t ret = read(g_dev_port, &c, 1);
		if(ret <= 0)
			return -1;
		else if(ret > 0)
			break;
	} while(1);
	return c;
}

int dev_gets(char *s,int size)
{
	int rd;
	size--;
	for(rd=0;rd<size;rd++){
		int c;
		c = dev_getchar();
		if(c < 0)
			return -1;
		s[rd] = c;
		if(c == 0xa){
			rd++;
			break;
		}
	}
	s[rd] = '\0';
	return rd;
}

void usage(){
	fprintf(stderr,"usage: asap [-d DEVICE-PATH] [-s SLEEP-COMMAND] [-c NOBODY] [-m NOBODY-DISTANCE]\n");
	exit(0);
}

void syg_handler(int arg)
{
	dev_close();
	fprintf(stderr, " ^C\n");
}

int main (int argc, const char * argv[])
{
	int ch;
	extern char	*optarg;
	extern int	optind, opterr;
	
	char *device_path = NULL;
	char *sleep_cmd = NULL;
	
	int nobody_distance = 800;
	int nobody = 5;
	int nobody_count = 0;
	
	while ((ch = getopt(argc, argv, "c:d:m:s:")) != -1) {
		switch (ch) {
			case 'c':
				nobody = atoi(optarg);
			    break;
			case 'd':
				device_path = optarg;
				break;
			case 'm':
				nobody_distance = atoi(optarg);
			    break;
			case 's':
				sleep_cmd = optarg;
				break;
		}
	}
	
	if (device_path == NULL || sleep_cmd == NULL) {
		usage();
	}
	
	if (dev_open(device_path) < 0){
		return 1;
	}
	
	signal(SIGINT, syg_handler);
	
	printf("waiting for Arduino.\n");
	sleep(5);
	dev_flush();
	
	while(1){
		char buf[1024];
		int distance;
		
		dev_flush();
		dev_putchar('*');
		dev_gets(buf, sizeof(buf));
		
		distance = atoi(buf);
		
		if (distance > nobody_distance){
			printf("%d mm, now we will sleep %d\n", distance, (nobody - nobody_count));
			nobody_count++;
			if (nobody_count >= nobody) {
				dev_putchar('e'); // Enable Remote wakeup
				
				printf("good night...\n");
				system(sleep_cmd);
				sleep(5);
				
				dev_putchar('d'); // Disable Remote wakeup
				nobody_count = 0;
			}
		}else{
			nobody_count = 0;
			printf("%d mm.\n", distance);
		}
		sleep(1);
	}
	
	dev_close();
    return 0;
}
