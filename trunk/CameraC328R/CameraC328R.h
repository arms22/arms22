/**
 * Copyright 2009
 * Sean Voisen <http://gizmologi.st>
 * Beatriz da Costa <http://beatrizdacosta.net>
 *
 * Based on the .NET driver authored by Pavel Bansky <http://banksy.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 *
 *   http://www.apache.org/licenses/LICENSE-2.0 
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License. 
 */

#ifndef CameraC328R_h
#define CameraC328R_h

#include "WProgram.h"
#include "WConstants.h"
#include <inttypes.h>

// The byte size of commands
#define CMD_SIZE 6

// Number of sync attempts to try before giving up
#define MAX_SYNC_ATTEMPTS 60

// How long to wait for serial communication responses
#define RESPONSE_DELAY 100

// How long to wait for camera to process JPEG data
#define PROCESS_DELAY 1000

// How long to wait between data packages
#define PACKAGE_DELAY 10

// The default size of data packages when retrieving
// JPEG image data
#define DEFAULT_PACKAGE_SIZE 64

// The byte offset where image data starts in a JPEG image
// data package
#define PACKAGE_DATA_START 4

// The byte offset from the end of a data package where
// JPEG image data ends
#define PACKAGE_DATA_END_OFFSET 2

// Maximum allowed errors when reading picture data
#define MAX_ERRORS 15


/**
 * Provides a driver interface to the C328R camera from COMedia Ltd.
 */
class NewSoftSerial;
class CameraC328R
{
  public:
    enum ColorType 
    { 
      CT_GRAYSCALE_2 = 0x01, 
      CT_GRAYSCALE_4 = 0x02, 
      CT_GRAYSCALE_8 = 0x03, 
      CT_COLOR_12 = 0x05, 
      CT_COLOR_16 = 0x06, 
      CT_JPEG = 0x07 
    };

    enum PreviewResolution
    {
      PR_80x60 = 0x01,
      PR_160x120 = 0x03
    };

    enum JPEGResolution
    {
      JR_80x64 = 0x01,
      JR_160x128 = 0x03,
      JR_320x240 = 0x05,
      JR_640x480 = 0x07
    };

    enum SnapshotType
    {
      ST_COMPRESSED = 0x00,
      ST_UNCOMPRESSED = 0x01
    };

    enum PictureType
    {
      PT_SNAPSHOT = 0x01,
      PT_PREVIEW = 0x02,
      PT_JPEG = 0x05
    };

    enum FrequencyType
    {
      FT_50Hz = 0x00,
      FT_60Hz = 0x01
    };

    enum BaudRate
    {
      BAUD7200 = 0xFF,
      BAUD9600 = 0xBF,
      BAUD14400 = 0x7F,
      BAUD19200 = 0x5F,
      BAUD28800 = 0x3F,
      BAUD38400 = 0x2F,
      BAUD57600 = 0x1F,
      BAUD115200 = 0x0F
    };
	
	CameraC328R(NewSoftSerial *softSerial = NULL);
	
    bool sync();
    bool reset( bool );
    bool powerOff();
    bool initial( ColorType, PreviewResolution, JPEGResolution );
    bool setLightFrequency( FrequencyType );
    bool setPackageSize( uint16_t );
    bool snapshot( SnapshotType, uint16_t );
    bool getJPEGPicture( PictureType, uint16_t, void (*)(uint16_t, uint16_t, uint16_t, byte*) );
    bool setBaudRate( BaudRate );
    bool getRawPicture( PictureType, byte[], uint16_t&, uint16_t );

  private:
	NewSoftSerial *_serialPort;
    uint16_t _packageSize;
    byte _command[CMD_SIZE];
    byte _receive_cmd[CMD_SIZE];
    void createCommand( const byte, byte, byte, byte, byte );
    void sendCommand();
    bool waitForResponse( uint32_t, byte[], uint16_t );
    bool waitForResponse( uint32_t );
    bool waitForACK( uint32_t, uint8_t );
    void sendACK( const byte, uint16_t );
    void sendACK( const byte );
    bool getPicture( PictureType, uint16_t, uint16_t& );
	void serial_flush(void);
	uint8_t serial_available(void);
	void serial_print(uint8_t);
	int serial_read(void);
};

#endif
