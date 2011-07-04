/*
  Warning: PLEASE DO NOT REMOVE THIS COMMENT WHEN REDISTRIBUTING!
  This is the second official release of the Phi-big-font library version 20110617
  Released for Phi-1 and Phi-2 shields for Arduino
  User is encouraged to obtain a Phi shield or is solely responsible for converting it to work on other shields or configurations.
  
  Programmed by Dr. John Liu
  Revision: 06/17/2011
  Free software for educational and personal uses.
  No warrantee!
  Commercial use without authorization is prohibited.
  Find details of the Phi-1 shield, Phi-2 shield, and Phi-big-font or contact Dr. Liu at
  http://liudr.wordpress.com/phi-1-shield/
  http://liudr.wordpress.com/phi-2-shield/
  http://liudr.wordpress.com/phi_big_font/
  All rights reserved.
  
  Updates:
  The array names for programmable characters on the LCD has been changed to aviod conflict with other libraries. 
*/
#include <WProgram.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include "phi_big_font.h"

static PROGMEM prog_char pbf_lcd_ch0[]	= {64,64,64,64,64,64,64,64,0};	//0
static PROGMEM prog_char pbf_lcd_ch1[]	= {64,64,64,64,64,31,31,31,0};	//1
static PROGMEM prog_char pbf_lcd_ch2[]	= {64,64,64,31,31,64,64,64,0};	//2
static PROGMEM prog_char pbf_lcd_ch3[]	= {64,64,64,31,31,31,31,31,0};	//3
static PROGMEM prog_char pbf_lcd_ch4[]	= {31,31,31,64,64,64,64,64,0};	//4
static PROGMEM prog_char pbf_lcd_ch5[]	= {31,31,31,64,64,31,31,31,0};	//5
static PROGMEM prog_char pbf_lcd_ch6[]	= {31,31,31,31,31,64,64,0};		//6
static PROGMEM prog_char pbf_lcd_ch7[]	= {31,31,31,31,31,31,31,31,0};	//7

static PROGMEM const char *pbf_ch_item[] = {
	pbf_lcd_ch0,
	pbf_lcd_ch1,
	pbf_lcd_ch2,
	pbf_lcd_ch3,
	pbf_lcd_ch4,
	pbf_lcd_ch5,
	pbf_lcd_ch6,
	pbf_lcd_ch7
};

static PROGMEM prog_char font_35_09[] = {
	1,2,3,7,1,6, 1,3,0,1,7,1, 2,2,3,7,5,5, 2,2,3,5,5,7,
	3,0,3,4,4,7, 3,2,2,5,5,7, 3,2,2,7,5,7, 2,2,3,0,0,7,
	3,2,3,7,5,7, 3,2,3,5,5,7
};

static PROGMEM prog_char font_35_AZ[] = {
	3,2,3,7,4,7, 3,2,3,7,5,3, 3,2,2,7,1,1, 3,2,1,7,1,6,
	3,2,2,7,5,5, 3,2,2,7,4,4, 3,2,2,7,1,7, 3,0,3,7,4,7,
	2,3,2,1,7,1, 2,2,3,3,1,7, 3,0,3,7,4,5, 3,0,0,7,1,1,
	3,1,3,7,4,7, 3,2,3,7,0,7, 3,2,3,7,1,7, 3,2,3,7,4,4,
	3,2,3,4,4,7, 3,2,3,7,4,3, 3,2,2,5,5,7, 2,3,2,0,7,0,
	3,0,3,7,1,7, 3,0,3,6,1,6, 3,0,3,7,6,7, 3,0,3,3,4,3,
	3,0,3,4,7,4, 2,2,3,3,5,1
};

static PROGMEM prog_char font_35_az[] = {
	2,2,1,2,5,7, 3,1,0,7,1,6, 1,2,2,6,1,1, 0,1,3,6,1,7,
	1,2,1,6,5,5, 0,1,2,4,7,4, 1,2,1,1,5,6, 3,1,0,7,0,7,
	0,2,0,0,7,0, 0,0,2,3,1,6, 3,0,1,7,4,3, 3,0,0,6,1,1,
	3,3,1,7,4,7, 3,2,1,7,0,7, 1,2,1,6,1,6, 3,2,1,7,4,0,
	1,2,3,0,4,7, 1,2,2,7,0,0, 1,2,2,1,5,2, 1,3,1,0,6,1,
	3,0,3,6,1,7, 3,0,3,6,1,6, 3,0,3,6,7,7, 3,0,3,3,4,3,
	3,0,3,1,5,6, 2,2,3,3,5,1
};

static LiquidCrystal *output_lcd = NULL;
static boolean inverted_big_font = false;

static void read_big_char_code(char ch, uint8_t *buf)
{
	prog_char *from;
	uint8_t i;
	if ( (ch >= '0') && (ch <= '9')) {
		ch = ch - '0';
		from = font_35_09;
	}
	else if ( (ch >= 'A') && (ch <= 'Z') ) {
		ch = ch - 'A';
		from = font_35_AZ;
	}
	else if ( (ch >= 'a') && (ch <= 'z')) {
		ch = ch - 'a';
		from = font_35_az;
	}
	else {
		i = 0;
		for (; i<3; i++){
			buf[i] = 3;
		}
		for (; i<6; i++){
			buf[i] = 7;
		}
		return;
	}
	uint8_t off = ch * 6;
	for (i=0; i<6; i++){
		buf[i] = pgm_read_byte(from + off + i);
	}
}

static void render_big_char_half(uint8_t *dat)
{
	if (!inverted_big_font) {
		for (uint8_t i=0; i<3; i++){
			output_lcd->write(dat[i]);
		}
		output_lcd->write(' ');
	}else{
		for (uint8_t i=0; i<3; i++){
			output_lcd->write(7 - dat[i]);
		}
		output_lcd->write(255);
	}
}

void render_big_char(char ch, byte loc_x, byte loc_y)
{
	uint8_t dat[6];
	read_big_char_code(ch, dat);
	
	output_lcd->setCursor(loc_x,loc_y);
	render_big_char_half( &dat[0] );
	
	output_lcd->setCursor(loc_x,loc_y+1);
	render_big_char_half( &dat[3] );
}

// void render_big_msg(char msg[], byte loc_x, byte loc_y)
// {
//   byte i=0;
//   while(msg[i])
//   {
//     render_big_char(msg[i],loc_x+i*4, loc_y);
//     i++;
//   }
// }

// void render_big_number(int number, byte loc_x, byte loc_y)
// {
//   char msg[11];
//   sprintf(msg,"%d",number);
//   render_big_msg(msg, loc_x, loc_y);
// }

//Display strings stored in PROGMEM. Provide the string name stored in PROGMEM to be displayed on the LCD's current cursor position.
// void big_msg_lcd(char* msg_line, byte loc_x, byte loc_y)
// {
//   char msg_buffer[17];
//   strcpy_P(msg_buffer,msg_line);
//   msg_buffer[4]=0; 
//   render_big_msg(msg_buffer, loc_x, loc_y);
// }

void lcd_clear()
{
  if (!inverted_big_font) output_lcd->clear();
  else
  {
    output_lcd->setCursor(0,0);
    for (byte i=0;i<80;i++)
    {
      output_lcd->write(255);
    }
  }
}

void invert_big_font(boolean inv)
{
  inverted_big_font = inv;
}

void init_big_font(LiquidCrystal *l)
{
  byte ch_buffer[10]; // This buffer is required for custom characters on the LCD.
  output_lcd=l;
  for (int i=0;i<8;i++)
  {
    strcpy_P((char*)ch_buffer,(char*)pgm_read_word(&(pbf_ch_item[i])));
    output_lcd->createChar(i, ch_buffer);
  }
}

// void display_img(byte msg[])
// {
//   output_lcd->setCursor(0,0);
//   for (byte i=0;i<80;i++)
//   {
//     output_lcd->write(msg[i]);
//   }
// }
