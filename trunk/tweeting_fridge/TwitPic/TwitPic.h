/*
  TwitPic.h - TwitPic Image Uploader for Arduino.
  Copyright (c) arms22 2009-2010. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef TwitPic_h
#define TwitPic_h

#include <Ethernet.h>
#include <avr/pgmspace.h>

class TwitPic
{
public:
	TwitPic(const char *username,
			const char *password);
	int uploadAndPost(const char *message,
					  uint32_t (*imageTransfer)(Client *client));
	int upload(uint32_t (*imageTransfer)(Client*client));
	int waitRspStat(void);
private:
	int read(char *buf,int size);
	int read_until_match_P(const prog_char *str);
	void print_P(const prog_char *str);
	void println_P(const prog_char *str);
	const char *username;
	const char *password;
	Client client;	
};

#endif
