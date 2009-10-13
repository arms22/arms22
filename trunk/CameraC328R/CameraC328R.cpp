/**
 * Copyright 2009 
 * Sean Voisen <http://gizmologi.st>
 * Beatriz da Costa <http://beatrizdacosta.net>
 *
 * Based on the .NET driver authored by Pavel Bansky <http://bansky.net>
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

#include "CameraC328R.h"
#include <NewSoftSerial.h>

/******************************************************************************
 * Constants
 *****************************************************************************/
static const byte CMD_PREFIX = 0xAA;
static const byte CMD_SYNC = 0x0D;
static const byte CMD_ACK = 0x0E;
static const byte CMD_NAK = 0x0F;
static const byte CMD_INITIAL = 0x01;
static const byte CMD_DATA = 0x0A;
static const byte CMD_RESET = 0x08;
static const byte CMD_POWEROFF = 0x09;
static const byte CMD_BAUDRATE = 0x07;
static const byte CMD_PACKAGESIZE = 0x06;
static const byte CMD_SNAPSHOT = 0x05;
static const byte CMD_GETPICTURE = 0x04;
static const byte CMD_LIGHTFREQ = 0x13;

static const uint16_t LAST_JPEG_ACK = 0xF0F0;
static const byte RAW_ACK = 0x0A;

/******************************************************************************
 * Methods
 *****************************************************************************/

/**
 * Constructor.
 */
CameraC328R::CameraC328R(NewSoftSerial *softSerial)
{
  _serialPort = softSerial;
  _packageSize = DEFAULT_PACKAGE_SIZE;
}

/**
 * Performs synchronization with the camera. Synchronization will be attempted 
 * up to MAX_SYNC_ATTEMPTS. Before you can do anything with the camera, you
 * must synchronize! Call this method first.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::sync()
{
  uint8_t attempts = 0;
  bool success;
  
  // Create the sync command
  createCommand( CMD_SYNC, 0, 0, 0, 0 );

  while( attempts < MAX_SYNC_ATTEMPTS ) 
  {
    // Send a SYNC command
    sendCommand();

    // Wait for ACK response
    success = waitForACK( RESPONSE_DELAY, CMD_SYNC );

    // Make sure it is an ACK
    if( success )
    {
      // Now wait for a SYNC
      success = waitForResponse( RESPONSE_DELAY );
      if( success && _receive_cmd[1] == CMD_SYNC )
      {
        // All good, flush the buffer
        serial_flush();

        // Now send an ACK
        createCommand( CMD_ACK, CMD_SYNC, 0, 0, 0 );
        sendCommand();

        return true;
      }
    }

    attempts++;
  }

  return false;
}

/**
 * Sets up the camera's initial image size and color type. You will
 * want to call this method after synchronization to set up your photo
 * parameters before taking a snapshot.
 *
 * @param colorType The color type setting for all photos. This should
 * be a value from the ColorType enumeration.
 * @param previewResolution The resolution to use for preview images.
 * This should be a value from the PreviewResolution enumeration.
 * @param jpegResolution The resolution to use for JPEG photos. This 
 * should be a value from the JPEGResolution enumeration.
 *
 * @return True if successful, false otherwise.
 */
bool CameraC328R::initial( ColorType colorType, PreviewResolution previewResolution, JPEGResolution jpegResolution )
{
  createCommand( CMD_INITIAL, 0, colorType, previewResolution, jpegResolution );
  sendCommand();

  if( waitForACK( RESPONSE_DELAY, CMD_INITIAL ) )
  {
    return true;
  } 

  return false;
}

/**
 * Sets the light frequency setting.
 *
 * @param frequencyType The frequency type to use. This should be a
 * value from the FrequencyType enumeration.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::setLightFrequency( FrequencyType frequencyType )
{
  createCommand( CMD_LIGHTFREQ, (byte)frequencyType, 0, 0, 0 );
  sendCommand();

  if( waitForACK( RESPONSE_DELAY, CMD_LIGHTFREQ ) )
  {
    return true;
  }

  return false;
}

/**
 * Sets the size of the package for image data.
 *
 * @param size The package size to use, in bytes. This can be a value between 64
 * and 512.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::setPackageSize( uint16_t size )
{
  createCommand( CMD_PACKAGESIZE, 0x08, (byte)(size & 0xFF), (byte)(size >> 8), 0 );
  sendCommand();

  if( waitForACK( RESPONSE_DELAY, CMD_PACKAGESIZE ) )
  {
    // Store package size in instance for future reference
    _packageSize = size;
    return true;
  }

  return false;
}

/**
 * Takes a snapshot with the camera and stores it in the camera's
 * data buffer. Once a snapshot has been taken, use getJPEGPicture or
 * getRawPicture to retrieve the photo data.
 *
 * @param snapshotType The snapshot type, compressed or uncompressed. This
 * should be a value from the SnapshotType enumeration.
 * @param skipFrames The number of frames to drop before compression occurs.
 * 0 keeps the first frame, 1 the second, and so forth.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::snapshot( SnapshotType snapshotType, uint16_t skipFrames )
{
  createCommand( CMD_SNAPSHOT, snapshotType, (byte)(skipFrames & 0xFF), (byte)(skipFrames >> 8), 0 ); 
  sendCommand();

  if( waitForACK( RESPONSE_DELAY, CMD_SNAPSHOT ) )
  {
    return true;
  }

  return false;
}

/**
 * Gets a snapshot from the camera in compressed JPEG format.
 * Use snapshot to take a picture first.
 *
 * @param pictureType The picture type. Should be either JPEG or snapshot
 * type from the PictureType enumeration.
 * @param processDelay The time wait to process the picture with the camera's
 * on board JPEG compression. Larger pictures can take up to 1 second.
 * @param callback This is a pointer reference to a function that returns void.
 * Because the Atmega's memory is limited, and its built-in EEPROM is even smaller,
 * use this callback to handle the chunks of image data as they arrive. Each time
 * a package containing image data is transmitted, this callback will be called.
 * You will probably want to write the data to external memory of some sort,
 * appending it each time.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::getJPEGPicture( PictureType pictureType, uint16_t processDelay, void (*callback)( uint16_t picSize, uint16_t packPicDataSize, uint16_t packCount, byte* pack ) )
{
  uint16_t pictureSize = 0;

  if( !getPicture( pictureType, processDelay, pictureSize ) )
    return false;

  // Keeps track of the number of errors, will wait up till MAX_ERRORS
  uint8_t errorCount = 0;
  // Keeps track of the number of packages received
  uint16_t packageCount = 0;
  // The size of the picture data within the package
  uint16_t packagePictureDataSize = 0;
  // Current byte position in the ENTIRE image
  uint16_t pictureDataPosition = 0; 

  byte package[_packageSize];
  bool packageSuccess;

  // Wait for MAX_ERRORS before aborting
  while( pictureDataPosition < pictureSize && errorCount < MAX_ERRORS )
  {
    sendACK( 0, packageCount );

    // Give it time to process and transmit data
    delay( PACKAGE_DELAY );

    // Wait for the package
    packageSuccess = waitForResponse( processDelay, package, _packageSize );

    if( packageSuccess )
    {
      // This is actually the size of the picture
      // data within the package
      packagePictureDataSize = package[3] << 8;
      packagePictureDataSize |= package[2];

      // Call the callback
      // Add PACKAGE_DATA_START to package address so user can begin grabbing
      // picture data without worrying about 4 byte offset
      callback( pictureSize, packagePictureDataSize, packageCount, package + PACKAGE_DATA_START );
      
      pictureDataPosition += packagePictureDataSize;
      packageCount++;
    }
    else
    {
      errorCount++;
    }
  }

  // Send last ACK
  sendACK( 0, LAST_JPEG_ACK );

  if( errorCount < MAX_ERRORS )
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Resets the camera.
 *
 * @param completeReset True to completely reset the camera, otherwise
 * a "soft" reset (reset the camera state machine) will occur
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::reset( bool completeReset )
{
  createCommand( CMD_RESET, completeReset ? 0x00 : 0x01, 0, 0, 0xFF );
  sendCommand();

  if( waitForACK( RESPONSE_DELAY, CMD_RESET ) )
  {
    return true;
  }
  
  return false;
}

/**
 * Sets the baud rate to use with the camera. This is useful if
 * you need to change the baud rate AFTER synchronization.
 *
 * @param baudRate The baud rate to use. This should be a value from
 * the BaudRate enumeration.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::setBaudRate( BaudRate baudRate )
{
  createCommand( CMD_BAUDRATE, (byte)baudRate, 0x01, 0, 0 );
  sendCommand();

  if( waitForACK( RESPONSE_DELAY, CMD_BAUDRATE ) )
  {
    return true;
  }

  return false;
}

/**
 * Gets a raw, uncompressed photo from the camera. Unlike getJPEGPicture,
 * this method returns the ENTIRE photo in one large data package. This
 * means that the photo must be temporarily stored in the Atmega's
 * memory. Use this method only if you have memory to spare.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::getRawPicture( PictureType pictureType, byte pictureBuffer[], uint16_t &bufferSize, uint16_t processDelay )
{
  uint16_t pictureSize = 0;

  if( !getPicture( pictureType, processDelay, pictureSize ) )
    return false;

  if( pictureSize > bufferSize )
    return false;
  else 
    bufferSize = pictureSize;

  // Wait for the package
  if( waitForResponse( processDelay, pictureBuffer, pictureSize ) ) {
    sendACK( RAW_ACK );
    return true;
  }

  return false;
}

/**
 * Power off the camera. The camera will be unusable after calling
 * this method until you successfully complete synchronization.
 *
 * @return True if successful, false otherwise
 */
bool CameraC328R::powerOff()
{
  createCommand( CMD_POWEROFF, 0, 0, 0, 0 );
  sendCommand();

  if( waitForACK( RESPONSE_DELAY, CMD_POWEROFF ) )
  {
    return true;
  }

  return false;
}

/**
 * @private
 *
 * Used by getJPEGPicture and getRawPicture to start the get picture
 * process. This method is used primarily to get the size of the picture
 * that is going to be transmitted by the camera.
 */
bool CameraC328R::getPicture( PictureType pictureType, uint16_t processDelay, uint16_t &pictureSize )
{
  pictureSize = 0;

  createCommand( CMD_GETPICTURE, pictureType, 0, 0, 0 );
  sendCommand();

  // Give the camera some time for processing
  delay( processDelay );

  if( !waitForACK( processDelay, CMD_GETPICTURE ) )
    return false;

  if( waitForResponse( processDelay ) && _receive_cmd[1] == CMD_DATA )
  {
    // Set the picture size for future reference
    pictureSize = _receive_cmd[5] << 8;
    pictureSize |= _receive_cmd[4] << 8;
    pictureSize |= _receive_cmd[3];

    return true;
  }

  return false;
}

/**
 * @private
 *
 * Waits for an ACK.
 *
 * @return True if got an ACK for the specified command, false otherwise
 */
bool CameraC328R::waitForACK( uint32_t timeout, uint8_t cmdId )
{
  bool success = waitForResponse( timeout );

  // TODO: We are ignoring NAKs here. Should we do something for this
  // specific case?
  if( success && _receive_cmd[1] == CMD_ACK && _receive_cmd[2] == cmdId )
  {
    return true;
  }

  return false;
}

/**
 * @private
 *
 * Sends the command that is stored in _command.
 */
void CameraC328R::sendCommand()
{
  uint8_t i;

  for( i = 0; i < CMD_SIZE; i++ )
  {
    serial_print( _command[i] );
  }
}

/**
 * @private
 *
 * Simple utility method for populating the _command array.
 */
void CameraC328R::createCommand( const byte cmd, byte param1, byte param2, byte param3, byte param4 )
{
  _command[0] = CMD_PREFIX;
  _command[1] = cmd;
  _command[2] = param1;
  _command[3] = param2;
  _command[4] = param3;
  _command[5] = param4;
}

/**
 * @private
 *
 * Sends and ACK.
 */
void CameraC328R::sendACK( const byte cmd )
{
  sendACK( cmd, 0 );
}

/**
 * @private
 *
 * Sends an ACK. Used when getting data packages to send an
 * ACK for each numbered package.
 */
void CameraC328R::sendACK( const byte cmd, uint16_t packageId )
{
  createCommand( CMD_ACK, cmd, 0, (byte)(packageId & 0xFF), (byte)(packageId >> 8) );
  sendCommand();
}

/**
 * @private
 *
 * Waits for a response up to timeout, and stores response in the
 * _receive_cmd array.
 *
 * @return True if response received, false otherwise
 */
bool CameraC328R::waitForResponse( uint32_t timeout )
{
  return waitForResponse( timeout, _receive_cmd, CMD_SIZE );
}

/**
 * @private
 *
 * Waits for a response up to timeout, and stores response in the
 * buffer array provided.
 *
 * @param timeout The timeout
 * @param buffer The buffer within which to store the response
 * @param bufferLength The length of the buffer provided
 *
 * @return True if response received, false otherwise
 */
bool CameraC328R::waitForResponse( uint32_t timeout, byte buffer[], uint16_t bufferLength )
{
  uint8_t byteCnt = 0;
  unsigned long time = millis();

  while( millis() - time <= timeout )
  {
    while( serial_available() > 0 )
    {
      buffer[byteCnt] = serial_read();
      byteCnt++;

      if( byteCnt == bufferLength )
      {
        return true;
      }
    }
  }

  if( byteCnt > 0 )
  {
    return true;
  }

  return false;
}

void CameraC328R::serial_flush(void)
{
	if(_serialPort){
		_serialPort->flush();
	}else{
		Serial.flush();
	}
}

uint8_t CameraC328R::serial_available(void)
{
	uint8_t ret;
	if(_serialPort){
		ret = _serialPort->available();
	}else{
		ret = Serial.available();
	}
	return ret;
}

void CameraC328R::serial_print(uint8_t data)
{
	if(_serialPort){
		_serialPort->print(data,BYTE);
	}else{
		Serial.print(data,BYTE);
	}
}

int CameraC328R::serial_read(void)
{
	int ret;
	if(_serialPort){
		ret = _serialPort->read();
	}else{
		ret = Serial.read();
	}
	return ret;
}
