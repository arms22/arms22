/*
  Warning: PLEASE DO NOT REMOVE THIS COMMENT WHEN REDISTRIBUTING!
  This is the first official release of the Phi-big-font library version 20110617
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
*/
#include <WProgram.h>
#include <LiquidCrystal.h>

void invert_big_font(boolean inv);
void lcd_clear();
//void big_msg_lcd(char* msg_line, byte loc_x, byte loc_y);
//void render_big_number(int number, byte loc_x, byte loc_y);
//void render_big_msg(char msg[], byte loc_x, byte loc_y);
void render_big_char(char ch, byte loc_x, byte loc_y);
void init_big_font(LiquidCrystal *l);
//void display_img(byte msg[]);
