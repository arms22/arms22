/* Arduino JPEGCamera Library
 * Copyright 2010 SparkFun Electronic
 * Written by Ryan Owens
 * Modified by arms22
 */

#include "JPEGCamera.h"

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
	sndRcvBuf[0] = 0x56;
	sndRcvBuf[1] = 0x00;
	sndRcvBuf[2] = 0x34;
	sndRcvBuf[3] = 0x01;
	sndRcvBuf[4] = 0x00;
	sendCommand(5);
	if( recvResponse(sndRcvBuf, 9, 200) == 9 ){
		//The size is in the last 2 characters of the response.
		//Parse them and convert the characters to an integer
		*size = (uint16_t)(sndRcvBuf[7]) << 8;
		*size += (uint16_t)(sndRcvBuf[8]) & 0x00FF;
		ret = true;
	}else{
		ret = false;
	}
	return ret;
}

//Reset the camera
bool JPEGCamera::reset(void)
{
	sndRcvBuf[0] = 0x56;
	sndRcvBuf[1] = 0x00;
	sndRcvBuf[2] = 0x26;
	sndRcvBuf[3] = 0x00;
	sendCommand(4);
	return (recvResponse(sndRcvBuf, 4, 1000) == 4);
}

//Take a new picture
bool JPEGCamera::takePicture(void)
{
	sndRcvBuf[0] = 0x56;
	sndRcvBuf[1] = 0x00;
	sndRcvBuf[2] = 0x36;
	sndRcvBuf[3] = 0x01;
	sndRcvBuf[4] = 0x00;
	sendCommand(5);
	return (recvResponse(sndRcvBuf, 5, 200) == 5);
}

//Stop taking pictures
bool JPEGCamera::stopPictures(void)
{
	sndRcvBuf[0] = 0x56;
	sndRcvBuf[1] = 0x00;
	sndRcvBuf[2] = 0x36;
	sndRcvBuf[3] = 0x01;
	sndRcvBuf[4] = 0x03;
	sendCommand(5);
	return (recvResponse(sndRcvBuf, 5, 200) == 5);
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
	//Send the command to get read_size bytes of data from the current address
	sndRcvBuf[0]  = 0x56;
	sndRcvBuf[1]  = 0x00;
	sndRcvBuf[2]  = 0x32;
	sndRcvBuf[3]  = 0x0C;
	sndRcvBuf[4]  = 0x00;
	sndRcvBuf[5]  = 0x0A;
	sndRcvBuf[6]  = 0x00;
	sndRcvBuf[7]  = 0x00;
	sndRcvBuf[8]  = (uint8_t)(address>>8);
	sndRcvBuf[9]  = (uint8_t)(address);
	sndRcvBuf[10] = 0x00;
	sndRcvBuf[11] = 0x00;
	sndRcvBuf[12] = (uint8_t)(read_size>>8);
	sndRcvBuf[13] = (uint8_t)(read_size);
	sndRcvBuf[14] = 0x00;
	sndRcvBuf[15] = 0x0A;
	sendCommand(16);
	//Read the response header.
	if( recvResponse(sndRcvBuf, 5, 200) == 5 ){
		//Now read the actual data and add it to the response string.
		//Return the number of characters in the response.
		return recvResponse(response, read_size, 800);
	}
	return 0;
}

//Set Image size
//NOTE: Need reset()
bool JPEGCamera::imageSize(JPEGCamera::ImageSize size)
{
	sndRcvBuf[0] = 0x56;
	sndRcvBuf[1] = 0x00;
	sndRcvBuf[2] = 0x31;
	sndRcvBuf[3] = 0x05;
	sndRcvBuf[4] = 0x04;
	sndRcvBuf[5] = 0x01;
	sndRcvBuf[6] = 0x00;
	sndRcvBuf[7] = 0x19;
	sndRcvBuf[8] = (uint8_t)size;
	sendCommand( 9 );
	return (recvResponse(sndRcvBuf, 5, 200) == 5);
}

//Send a command to the camera
//pre: cameraPort is a serial connection to the camera set to 38400 baud
//     sndRcvBuf is a string with the command to be sent to the camera
//	   length is the number of characters in the command
//usage: None (private function)
void JPEGCamera::sendCommand(uint8_t length)
{
	//Clear any data currently in the serial buffer
	cameraPort.flush();
	//Send each character in the command string to the camera through the camera serial port
	cameraPort.write(sndRcvBuf,length);
// 	for(uint8_t i=0; i<length; i++){
// 		cameraPort.write(sndRcvBuf[i]);
// 	}
}

//Recv a command from the camera
//usage: None (private function)
uint16_t JPEGCamera::recvResponse(uint8_t * response, uint16_t length, unsigned long timeout)
{
	unsigned long time = millis();
	uint16_t count = 0;
	while( (millis() - time <= timeout) &&
		   (count < length) ){
		if( cameraPort.available() ){
			*response++=cameraPort.read();
			count++;
		}
	}
	//return the number of characters in the response string
	return count;
}
