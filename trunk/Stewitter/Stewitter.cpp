/*
 Stewitter.cpp - Arduino library to Post messages to Twitter with OAuth.
 Copyright (c) arms22 2010 - 2012. All right reserved.
 */
/*
 Twitter.cpp - Arduino library to Post messages to Twitter.
 Copyright (c) NeoCat 2009. All right reserved.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#include "Stewitter.h"

#define STEWGATE_POST_API			"/sg1/post/"
#define STEWGATE_LAST_MENTION_API	"/sg1/last_mention/"
#define STEWGATE_HOST				"stewgate.appspot.com"

Stewitter::Stewitter(const char *token) : client(), token(token)
{
    httpBody.reserve(100);
}

bool Stewitter::post(const char *msg)
{
    statusCode = 0;
    parseStatus = 0;
    httpBody = "";
	if (client.connect(STEWGATE_HOST, 80)) {
		int length;
        if (msg != NULL) {
            client.println(F("POST " STEWGATE_POST_API " HTTP/1.0"));
        } else {
            client.println(F("POST " STEWGATE_LAST_MENTION_API " HTTP/1.0"));
        }
		client.println(F("Host: " STEWGATE_HOST));
		client.print(F("Content-Length: "));
        if (msg != NULL) {
            length = strlen(token) + strlen(msg) + 8;
        }else{
            length = strlen(token) + 3;
        }
		client.println(length);
		client.println();
		client.print(F("_t="));
		client.print(token);
        if (msg != NULL) {
            client.print(F("&msg="));
            client.print(msg);
        }
		client.println();
	} else {
		return false;
	}
	return true;
}

bool Stewitter::lastMention(void)
{
    httpBody.reserve(200);
    return post(NULL);
}

bool Stewitter::checkStatus(Print *debug)
{
    bool ret = true;
    
	if (client.available()) {
        char c = client.read();
        if (debug)
            debug->print(c);
        switch(parseStatus) {
            case 0: // skip "HTTP/1.1 "
                if (c == ' ') {
                    parseStatus++;
                }
                break;
            case 1: // parse Status Code
                if (c >= '0' && c <= '9') { 
                    statusCode *= 10;
                    statusCode += c - '0';
                } else {
                    parseStatus++;
                }
                break;
            case 2: // skip HTTP Headers
                if (c == '\x0a') {
                    parseStatus++;
                }
                break;
            case 3:
                if (c == '\x0d') {
                    parseStatus++;
                }else{
                    parseStatus = 2;
                }
                break;
            case 4:
                if (c == '\x0a') {
                    parseStatus++;
                }else{
                    parseStatus = 2;
                }
                break;
            case 5: // recv HTTP Body
                httpBody += c;
                break;
        }
    }else{
        if (!client.connected()) {
            client.flush();
            client.stop();
            ret = false;
        }
    }
    return ret;
}

int Stewitter::wait(Print *debug)
{
	while (checkStatus(debug));
	return statusCode;
}
