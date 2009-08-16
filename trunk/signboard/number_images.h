#ifndef NUMBER_IMAGES_H
#define NUMBER_IMAGES_H

const prog_uint8_t PROGMEM number_image_slush[] = { 
  0 , 0 , 0 , 64 , 64 , 32 , 32 , 16 , 16 , 8 , 8 , 4 , 4 , 2 , 2 , 0 };
const prog_uint8_t PROGMEM number_image_number_0[] = { 
  0 , 0 , 0 , 60 , 98 , 98 , 82 , 82 , 82 , 74 , 74 , 74 , 70 , 70 , 60 , 0 };
const prog_uint8_t PROGMEM number_image_number_1[] = { 
  0 , 0 , 0 , 16 , 24 , 30 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 16 , 126 , 0 };
const prog_uint8_t PROGMEM number_image_number_2[] = { 
  0 , 0 , 0 , 28 , 34 , 64 , 64 , 64 , 64 , 32 , 16 , 8 , 4 , 2 , 126 , 0 };
const prog_uint8_t PROGMEM number_image_number_3[] = { 
  0 , 0 , 0 , 28 , 34 , 64 , 64 , 32 , 24 , 32 , 64 , 64 , 64 , 34 , 28 , 0 };
const prog_uint8_t PROGMEM number_image_number_4[] = { 
  0 , 0 , 0 , 48 , 40 , 40 , 36 , 36 , 34 , 34 , 126 , 32 , 32 , 32 , 32 , 0 };
const prog_uint8_t PROGMEM number_image_number_5[] = { 
  0 , 0 , 0 , 62 , 2 , 2 , 2 , 30 , 32 , 64 , 64 , 64 , 64 , 34 , 28 , 0 };
const prog_uint8_t PROGMEM number_image_number_6[] = { 
  0 , 0 , 0 , 60 , 2 , 2 , 2 , 2 ,62 , 66 , 66 , 66 , 66 , 66 , 60 , 0 };
const prog_uint8_t PROGMEM number_image_number_7[] = { 
  0 , 0 , 0 , 126 , 64 , 64 , 64 , 32 , 32 , 16 , 16 , 8 , 8 , 4 , 4 , 0 };
const prog_uint8_t PROGMEM number_image_number_8[] = { 
  0 , 0 , 0 , 60 , 66 , 66 , 66 , 36 , 24 , 36 , 66 , 66 , 66 , 66 , 60 , 0 };
const prog_uint8_t PROGMEM number_image_number_9[] = { 
  0 , 0 , 0 , 60 , 66 , 66 , 66 , 66 , 98 , 92 , 64 , 64 , 64 , 32 , 28 , 0 };
const prog_uint8_t PROGMEM number_image_colon[] = { 
  0,  0 , 0 , 0 , 0 , 24 , 24 , 0 , 0 , 0 , 0 , 24 , 24 , 0 , 0 , 0 };

#ifndef MYIMAGESTRUCT
#define MYIMAGESTRUCT

struct MyImage {
  uint8_t width;
  uint8_t height;
  const prog_uint8_t *data;
};

#endif

const MyImage number_images[] = {
  { 
    8,16, number_image_slush   }
  ,
  { 
    8,16, number_image_number_0   }
  ,
  { 
    8,16, number_image_number_1   }
  ,
  { 
    8,16, number_image_number_2   }
  ,
  { 
    8,16, number_image_number_3   }
  ,
  { 
    8,16, number_image_number_4   }
  ,
  { 
    8,16, number_image_number_5   }
  ,
  { 
    8,16, number_image_number_6   }
  ,
  { 
    8,16, number_image_number_7   }
  ,
  { 
    8,16, number_image_number_8   }
  ,
  { 
    8,16, number_image_number_9   }
  ,
  { 
    8,16, number_image_colon   }
};

MyImage numberImages(const char c)
{
  return number_images[c - '/'];
}

#endif
