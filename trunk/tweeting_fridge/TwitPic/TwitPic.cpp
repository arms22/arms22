/*
  TwitPic.cpp - TwitPic Image Uploader for Arduino.
  Copyright (c) arms22 2009-2010. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include <string.h>
#include <stdlib.h>
#include "TwitPic.h"

#define TWITPIC_UPLADO_API_URL			"/api/upload"
#define TWITPIC_UPLOAD_AND_POST_API_URL "/api/uploadAndPost"
#define BOUNDARY						"oniudrArofyrarbilciPtiwT"
#define HEADER							"--" BOUNDARY
#define FOOTER							"--" BOUNDARY "--"
#define IMAGE_FILE_NAME					"photo_taken_with_arduino.jpg"
#define IMAGE_CONTENT_TYPE				"image/jpeg"

#define SERIAL_DEBUG 0
#if SERIAL_DEBUG
#include <HardwareSerial.h>
#define DEBUG_PRINT(c) Serial.print(c)
#define DEBUG_PRINTLN(c) Serial.println(c)
#else
#define DEBUG_PRINT(c)
#define DEBUG_PRINTLN(c)
#endif

static uint8_t server[] = {174, 36, 58, 233}; // twitpic.com

TwitPic::TwitPic(const char *username,
				 const char *password)
	: client(server, 80)
	, username(username)
	, password(password)
{
}

int TwitPic::uploadAndPost(const char *message,
						   uint32_t (*imageTransfer)(Client*client))
{
	int ret = -1;
	DEBUG_PRINT("connecting...");

	if (client.connect()) {
		DEBUG_PRINTLN("ok\r\npost image");

		uint32_t length = 81 + strlen(username) + 81 + strlen(password) + 32;
		if(message)
			length += 80 + strlen(message);
		if(imageTransfer)
			length += 175 + imageTransfer(0);

		if(message)
			println_P( PSTR("POST " TWITPIC_UPLOAD_AND_POST_API_URL " HTTP/1.0") );
		else
			println_P( PSTR("POST " TWITPIC_UPLADO_API_URL " HTTP/1.0") );
		println_P( PSTR("Content-Type: multipart/form-data; boundary=" BOUNDARY) );
		print_P( PSTR("Content-Length: ") );
		client.println(length);
		client.println();

		if(imageTransfer){
			println_P( PSTR(HEADER) );
			println_P( PSTR("Content-Disposition: file; name=\"media\"; filename=\"" IMAGE_FILE_NAME "\"") );
			println_P( PSTR("Content-Type: " IMAGE_CONTENT_TYPE) );
			println_P( PSTR("Content-Transfer-Encoding: binary") );
			client.println();
			imageTransfer(&client);
			client.println();
		}

		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"username\"") );
		client.println();
		client.println(this->username);

		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"password\"") );
		client.println();
		client.println(this->password);

		if(message){
			println_P( PSTR(HEADER) );
			println_P( PSTR("Content-Disposition: form-data; name=\"message\"") );
			client.println();
			client.println(message);
		}

		println_P( PSTR(FOOTER) );
		client.println();
		client.flush();

		DEBUG_PRINTLN("wait response");
		ret = waitRspStat();
		
		client.stop();
	}else{
		DEBUG_PRINTLN("fail");
	}
	return ret;
}

int TwitPic::upload(uint32_t (*imageTransfer)(Client*client))
{
	return uploadAndPost(0, imageTransfer);
}

int TwitPic::waitRspStat(void)
{
	int ret = -1;
	if(read_until_match_P( PSTR("rsp stat") )){ // stat or status
		if(read_until_match_P( PSTR("=\"") )){
			char buf[5];
			if(read(buf,4) == 4){
				if(strncmp_P(buf, PSTR("ok"), 2) == 0){
					ret = 0;
				}
				else if(strncmp_P(buf, PSTR("fail"), 4) == 0){
					if(read_until_match_P( PSTR("err code=\"") )){
						if(read(buf,4) == 4){
							ret = -atoi(buf);
						}
					}
				}
			}
		}
	}
	while(client.available()){
		char c = client.read();
		DEBUG_PRINT(c);
	}
	return ret;
}

int TwitPic::read(char *buf,int size)
{
	int ret = 0;
	while(client.connected() && (ret<size)) {
		while (client.available() && (ret<size)) {
			char c = client.read();
			DEBUG_PRINT(c);
			buf[ret++] = c;
		}
	}
	buf[ret] = 0;
	return ret;
}

int TwitPic::read_until_match_P(const prog_char *str)
{
	int pos = 0;
	char cc = pgm_read_byte(str+pos);
	while(client.connected() && cc) {
		while (client.available() && cc) {
			char c = client.read();
			DEBUG_PRINT(c);
			if(cc == c) {
				pos++;
			} else {
				pos=0;
			}
			cc = pgm_read_byte(str+pos);
		}
	}
	return pos;
}

void TwitPic::print_P(const prog_char *str)
{
	char c;
	while(c = pgm_read_byte(str++)){
		client.print(c);
	}
}

void TwitPic::println_P(const prog_char *str)
{
	print_P(str);
	client.println();
}
