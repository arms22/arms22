// カメラ通信
CameraC328R camera;

// 写真保存用
File picture;

void getJPEGPicture_callback( uint16_t pictureSize, uint16_t packageSize, uint16_t packageCount, byte* package )
{
  green(HIGH);
  picture.write(package, packageSize);
  green(LOW);
}

int photoNumber(void)
{
  int num;
  num = EEPROM.read(0);
  num |= EEPROM.read(1) << 8;
  num++;
  if( num > 9999 ){
    num = 0;
  }
  EEPROM.write(0,num & 0xff);
  EEPROM.write(1,num >> 8);
  return num;
}

bool getJPEGPicture(void)
{
  int photoNo;
  uint16_t pictureSize = 0;
  char buf[12];

  if( !camera.sync() ){
    goto camera_error;
  }

  if( !camera.initial( CameraC328R::CT_JPEG, CameraC328R::PR_160x120, CameraC328R::JR_640x480 ) ){
    goto camera_error;
  }

  if( !camera.setPackageSize( 64 ) ){
    goto camera_error;
  }

  if( !camera.setLightFrequency( CameraC328R::FT_50Hz ) ){
    goto camera_error;
  }

  attention(HIGH);
  if( !camera.snapshot( CameraC328R::ST_COMPRESSED, 0 ) ){
    goto camera_error;
  }
  attention(LOW);

  photoNo = photoNumber();
  //  snprintf(buf, sizeof(buf), "/%04d", (photoNo / 500) * 500);
  //  if( !FatFs.changeDirectory(buf) ){
  //    FatFs.changeDirectory("/");
  //    FatFs.createDirectory(&buf[1]);// skip '/'
  //  }
  //
  //  if( !FatFs.changeDirectory(buf) ){
  //    goto camera_error;
  //  }

  if( !FatFs.changeDirectory("/photos") ){
    goto camera_error;
  }

  snprintf(buf, sizeof(buf), "img%04d.jpg", photoNo);
  if( !FatFs.createFile(buf) ){
    goto camera_error;
  }

  if( !picture.open(buf) ){
    goto camera_error;
  }

  if( !camera.getJPEGPictureSize( CameraC328R::PT_SNAPSHOT, PROCESS_DELAY, pictureSize) ){
    goto camera_error;
  }

  picture.resize(pictureSize);

  if( !camera.getJPEGPictureData( &getJPEGPicture_callback) ){
    goto camera_error;
  }

  picture.close();
  return true;

  {
camera_error:
    picture.close();
    return false;
  }
}



