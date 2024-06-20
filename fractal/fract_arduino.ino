#include <pin_magic.h>
#include <registers.h>
#include <SPFD5408_Adafruit_GFX.h>
#include <SPFD5408_Adafruit_TFTLCD.h>
#include <SPFD5408_TouchScreen.h>
#include <SPFD5408_Util.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BLACK       0x0000            
#define BLUE        0x001F      
#define GREEN       0x07E0           
#define RED         0xF800           
#define WHITE       0xFFFF

#define dot_rad 1
#define dot_A_x 150
#define dot_A_y 310
#define dot_B_x 5
#define dot_B_y 40
#define dot_C_x 235
#define dot_C_y 5

uint32_t rnd_num = 0;
uint16_t x_pos = 90;
uint16_t y_pos = 120;
uint16_t x_pos_old;
uint16_t y_pos_old;


void to_A() {
  x_pos_old = x_pos;
  y_pos_old = y_pos;
  x_pos = (dot_A_x + x_pos) / 2;
  y_pos = (dot_A_y + y_pos) / 2;
  //tft.fillCircle(x_pos, y_pos, dot_rad, BLUE);
  tft.drawPixel(x_pos, y_pos, BLUE);
}

void to_B() {
  x_pos_old = x_pos;
  y_pos_old = y_pos;
  x_pos = (dot_B_x + x_pos) / 2;
  y_pos = (dot_B_y + y_pos) / 2;
  //tft.fillCircle(x_pos, y_pos, dot_rad, GREEN);
  tft.drawPixel(x_pos, y_pos, GREEN);
}

void to_C() {
  x_pos_old = x_pos;
  y_pos_old = y_pos;
  x_pos = (dot_C_x + x_pos) / 2;
  y_pos = (dot_C_y + y_pos) / 2;
  //tft.fillCircle(x_pos, y_pos, dot_rad, RED);
  tft.drawPixel(x_pos, y_pos, RED);
}

void pre_draw() {
  switch(random(0, 3)) {
    case 0: tft.drawLine(x_pos, y_pos, dot_A_x, dot_A_y, RED);
        to_A();
        delay(600);
        tft.fillCircle(x_pos, y_pos, dot_rad, BLUE);
        delay(600);
        tft.drawLine(x_pos_old, y_pos_old, dot_A_x, dot_A_y, WHITE);
        break;
    case 1: tft.drawLine(x_pos, y_pos, dot_B_x, dot_B_y, RED);
        to_B();
        delay(600);
        tft.fillCircle(x_pos, y_pos, dot_rad, BLUE);
        delay(600);
        tft.drawLine(x_pos_old, y_pos_old, dot_B_x, dot_B_y, WHITE);
        break;
    case 2: tft.drawLine(x_pos, y_pos, dot_C_x, dot_C_y, RED);
         to_C();
         delay(600);
         tft.fillCircle(x_pos, y_pos, dot_rad, BLUE);
         delay(600);
         tft.drawLine(x_pos_old, y_pos_old, dot_C_x, dot_C_y, WHITE);
         break;
      }
  
}


 
void setup(void) {
 Serial.begin(9600);

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(2);
  tft.fillScreen(WHITE);

  tft.fillCircle(dot_A_x, dot_A_y, 3, BLACK);
  tft.fillCircle(dot_B_x, dot_B_y, 3, BLACK);
  tft.fillCircle(dot_C_x, dot_C_y, 3, BLACK);

  //for(uint8_t i=0; i<30; i++) {
  //pre_draw();
  //}
}
 
void loop(void) {
  switch(random(0, 3)) {
        case 0: to_A(); break;
        case 1: to_B(); break;
        case 2: to_C(); break;
   }
   //tft.drawPixel(x_pos, y_pos, BLUE);
   delay(1);
}
