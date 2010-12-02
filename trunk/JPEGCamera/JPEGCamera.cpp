/* Arduino JPEGCamera Library
 * Copyright 2010 SparkFun Electronic
 * Written by Ryan Owens
 * Modified by arms22
 */

#include "JPEGCamera.h"

#define RESPONSE_TIME_OUT	(500)
#define DATA_TIME_OUT		(1000)

//Commands for the LinkSprite Serial JPEG Camera
// const char GET_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
// const char RESET_CAMERA[4] = {0x56, 0x00, 0x26, 0x00};
// const char TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
// const char STOP_TAKING_PICS[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
// const char READ_DATA[8] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00};

JPEGCamera::JPEGCamera(Stream &port)
	: cameraPort(port)
{
}

//Get the size of the image currently stored in the camera
bool JPEGCamera::getSize(uint16_t * size)
{
	bool ret;
	//Send the GET_SIZE command string to the
	cmdResBuffer[0] = 0x56;
	cmdResBuffer[1] = 0x00;
	cmdResBuffer[2] = 0x34;
	cmdResBuffer[3] = 0x01;
	cmdResBuffer[4] = 0x00;
	if( commandAndResponse(5, 9) ){
		//The size is in the last 2 characters of the response.
		//Parse them and convert the characters to an integer
		*size = (uint16_t)((cmdResBuffer[7]) << 8) |
			(uint16_t)(cmdResBuffer[8]);
		ret = true;
	}else{
		ret = false;
	}
	return ret;
}

//Reset the camera
bool JPEGCamera::reset(void)
{
	cmdResBuffer[0] = 0x56;
	cmdResBuffer[1] = 0x00;
	cmdResBuffer[2] = 0x26;
	cmdResBuffer[3] = 0x00;
	return commandAndResponse(4, 4);
}

//Take a new picture
bool JPEGCamera::takePicture(void)
{
	cmdResBuffer[0] = 0x56;
	cmdResBuffer[1] = 0x00;
	cmdResBuffer[2] = 0x36;
	cmdResBuffer[3] = 0x01;
	cmdResBuffer[4] = 0x00;
	return commandAndResponse(5, 5);
}

//Stop taking pictures
bool JPEGCamera::stopPictures(void)
{
	cmdResBuffer[0] = 0x56;
	cmdResBuffer[1] = 0x00;
	cmdResBuffer[2] = 0x36;
	cmdResBuffer[3] = 0x01;
	cmdResBuffer[4] = 0x03;
	return commandAndResponse(5, 5);
}

//Get the read_size bytes picture data from the camera
//pre: response is an empty string, address is the address of data to grab
//post: response contains the cameras response to the readData command, but the response header is parsed out.
//returns: number of characters in the response
//Usage: camera.readData(response, cur_address);
//NOTE: This function must be called repeatedly until all of the data is read
//See Sample program on how to get the entire image.
uint16_t JPEGCamera::readData(uint8_t * response, uint16_t read_size, uint16_t address)
{
	uint16_t count = 0;
	//Send the command to get read_size bytes of data from the current address
	cmdResBuffer[0]  = 0x56;
	cmdResBuffer[1]  = 0x00;
	cmdResBuffer[2]  = 0x32;
	cmdResBuffer[3]  = 0x0C;
	cmdResBuffer[4]  = 0x00;
	cmdResBuffer[5]  = 0x0A;
	cmdResBuffer[6]  = 0x00;
	cmdResBuffer[7]  = 0x00;
	cmdResBuffer[8]  = (uint8_t)(address>>8);
	cmdResBuffer[9]  = (uint8_t)(address);
	cmdResBuffer[10] = 0x00;
	cmdResBuffer[11] = 0x00;
	cmdResBuffer[12] = (uint8_t)(read_size>>8);
	cmdResBuffer[13] = (uint8_t)(read_size);
	cmdResBuffer[14] = 0x00;
	cmdResBuffer[15] = 0x0A;
	//Send command and Recv the response header.
	if( commandAndResponse(16, 5) ){
		//Now read the actual data and add it to the response string.
		//Return the number of characters in the response.
		count = recvResponse(response, read_size, DATA_TIME_OUT);
		//Recv footer
		//recvResponse(cmdResBuffer, 5, RESPONSE_TIME_OUT);
	}
	return count;
}

//Set Image size
//NOTE: Need reset()
bool JPEGCamera::imageSize(JPEGCamera::ImageSize size)
{
	cmdResBuffer[0] = 0x56;
	cmdResBuffer[1] = 0x00;
	cmdResBuffer[2] = 0x31;
	cmdResBuffer[3] = 0x05;
	cmdResBuffer[4] = 0x04;
	cmdResBuffer[5] = 0x01;
	cmdResBuffer[6] = 0x00;
	cmdResBuffer[7] = 0x19;
	cmdResBuffer[8] = (uint8_t)size;
	return commandAndResponse(9, 5);
}

//Send a command to the camera
//pre: cameraPort is a serial connection to the camera set to 38400 baud
//     cmdResBuffer is a string with the command to be sent to the camera
//	   length is the number of characters in the command
//usage: None (private function)

uint8_t JPEGCamera::commandAndResponse(uint8_t cmd_sz,uint8_t res_sz)
{
	//Clear any data currently in the serial buffer
	cameraPort.flush();
	//Send each character in the command string to the camera through the camera serial port
	cameraPort.write(cmdResBuffer, cmd_sz);
	return recvResponse(cmdResBuffer, res_sz, RESPONSE_TIME_OUT);
}

//Recv a response from the camera
uint8_t JPEGCamera::recvResponse(uint8_t * response, uint8_t size, uint16_t timeout)
{
	unsigned long start_time;
	uint8_t count;
	start_time = millis();
	count = 0;
	do{
		if( cameraPort.available() ){
			*response++ = cameraPort.read();
			count++;
		}
	} while( (millis() - start_time <= timeout) && (count < size) );
	return count;
}
