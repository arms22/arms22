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



// Your TwitPic API Key
#define TWITPIC_API_KEY                 "6fb3a281fe292fa5fde5dd794f2e05da"

// Your Twitter Consumer key/Consumer Secret
#define CONSUMER_KEY                    "pPdNoeBBk4nsKNBFFzl0lg"
#define CONSUMER_SECRET                 "uwGtK2CFS2dWkAtuGtBiZKcB7SE35bWTJIHQlciJQs"

// Your Twitter Access Token (oauth_token)/Access Token Secret (oauth_token_secret)
#define ACCESS_TOKEN                    "7680122-ubBoUYUBhzkQwYFzET29pn9VJwXxzWQOHfGOCnlsKA"
#define ACCESS_TOKEN_SECRET             "aT1uVbk9Anw8ozZ0yVy4YP9QEvt9hSU7D1mgPpyYyJg"



#define TWITPIC_UPLADO_API_URL			"/1/upload.xml"
#define TWITPIC_UPLADO_AND_POST_API_URL "/1/uploadAndPost.xml"

#define BOUNDARY						"oniudrArofyrarbilciPtiwT"
#define HEADER							"--" BOUNDARY
#define FOOTER							"--" BOUNDARY "--"
#define IMAGE_FILE_NAME					"photo_taken_with_arduino.jpg"
#define IMAGE_CONTENT_TYPE				"image/jpeg"
#define CRLF							"\r\n"

#define SERIAL_DEBUG 1
#if SERIAL_DEBUG
#include <HardwareSerial.h>
#define DEBUG_PRINT(c)   Serial.print(c)
#define DEBUG_PRINTLN(c) Serial.println(c)
#else
#define DEBUG_PRINT(c)
#define DEBUG_PRINTLN(c)
#endif

static uint8_t server[] = {174, 36, 58, 233}; // api.twitpic.com

TwitPic::TwitPic()
	: client(server, 80)
{
}

int TwitPic::uploadAndPost(const char *message,
						   uint32_t (*imageTransfer)(Client*client),
						   bool post)
{
	int ret = -1;
	DEBUG_PRINT("connecting...");
	
	if (client.connect()) {
		DEBUG_PRINTLN("ok\r\npost image");
		
		uint32_t length = 0;
		length += 73 + sizeof("key"             TWITPIC_API_KEY) - 1;
		length += 73 + sizeof("consumer_token"  CONSUMER_KEY) - 1;
		length += 73 + sizeof("consumer_secret" CONSUMER_SECRET) - 1;
		length += 73 + sizeof("oauth_token"     ACCESS_TOKEN) - 1;
		length += 73 + sizeof("oauth_secret"    ACCESS_TOKEN_SECRET) - 1;
		length += 80 + strlen(message);
 		//length += 175 + imageTransfer(0);
		length += 32;
		
		if(post){
			println_P( PSTR("POST " TWITPIC_UPLADO_AND_POST_API_URL " HTTP/1.0") );
		}else{
			println_P( PSTR("POST " TWITPIC_UPLADO_API_URL " HTTP/1.0") );
		}
		println_P( PSTR("Content-Type: multipart/form-data; boundary=" BOUNDARY) );
		print_P(   PSTR("Content-Length: ") );
		println(length);
		println();
		
		// Post Parameter
		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"key\"") );
		println();
		println_P( PSTR(TWITPIC_API_KEY) );
		
		// Post Parameter
		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"consumer_token\"") );
		println();
		println_P( PSTR(CONSUMER_KEY) );
		
		// Post Parameter
		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"consumer_secret\"") );
		println();
		println_P( PSTR(CONSUMER_SECRET) );
		
		// Post Parameter
		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"oauth_token\"") );
		println();
		println_P( PSTR(ACCESS_TOKEN) );
		
		// Post Parameter
		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"oauth_secret\"") );
		println();
		println_P( PSTR(ACCESS_TOKEN_SECRET) );
		
		// Post Parameter
		println_P( PSTR(HEADER) );
		println_P( PSTR("Content-Disposition: form-data; name=\"message\"") );
		println();
		println(message);
		
		// Image Data
		if(imageTransfer){
			println_P( PSTR(HEADER) );
			println_P( PSTR("Content-Disposition: file; name=\"media\"; filename=\"" IMAGE_FILE_NAME "\"") );
			println_P( PSTR("Content-Type: " IMAGE_CONTENT_TYPE) );
			println_P( PSTR("Content-Transfer-Encoding: binary") );
			println();
			imageTransfer(&client);
			println();
		}
		
		// Footer
		println_P( PSTR(FOOTER) );
		println();
		
		// Wait Response
		client.flush();
		ret = waitResponses();
		client.stop();
	}else{
		DEBUG_PRINTLN("fail");
	}
	return ret;
}

int TwitPic::upload(const char *message,
					uint32_t (*imageTransfer)(Client*client))
{
	return uploadAndPost(message, imageTransfer, false);
}

int TwitPic::waitResponses(void)
{
	int ret = -1;
	DEBUG_PRINTLN("waitResponses");
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
		DEBUG_PRINT(c);
	}
}

void TwitPic::println_P(const prog_char *str)
{
	print_P(str);
	client.println();
	DEBUG_PRINTLN("");
}

void TwitPic::println(const char *str)
{
	client.println(str);
	DEBUG_PRINTLN(str);
}

void TwitPic::println(unsigned long d)
{
	client.println(d);
	DEBUG_PRINTLN(d);
}

void TwitPic::println(void)
{
	client.println();
	DEBUG_PRINTLN("");
}
