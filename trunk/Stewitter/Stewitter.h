/*
  Stewitter.h - Arduino library to Post messages to Twitter with OAuth.
  Copyright (c) arms22 2010 - 2012. All right reserved.
*/
/*
  Twitter.h - Arduino library to Post messages to Twitter.
  Copyright (c) NeoCat 2009. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef Stewitter_H
#define Stewitter_H

#include <Arduino.h>
#include <Ethernet.h>

class Stewitter
{
private:
	EthernetClient client;
    String httpBody;
	const char *token;
	int statusCode;
	uint8_t parseStatus;
public:
	Stewitter(const char *token);
	bool post(const char *msg);
    bool lastMention(void);
	bool checkStatus(Print *debug = NULL);
	int  wait(Print *debug = NULL);
	int  status(void) { return statusCode; }
    String& response(void) { return httpBody; }
};

#endif	//Stewitter_H
