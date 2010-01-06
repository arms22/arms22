/*
  Stewitter.h - Arduino library to Post messages to Twitter with OAuth.
  Copyright (c) arms22 2010. All right reserved.
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

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <Ethernet.h>

class Stewitter
{
private:
	uint8_t parseStatus;
	int statusCode;
	Client client;
	const char *deviceToken;
	void print_P(const prog_char *str);
	void println_P(const prog_char *str);
	void printPercentEscaped(const char *str);
public:
	Stewitter(const char *user_and_passwd);
	
	bool post(const char *msg);
	bool checkStatus(void);
	int  wait(void);
	int  status(void) { return statusCode; }
};

#endif	//Stewitter_H
