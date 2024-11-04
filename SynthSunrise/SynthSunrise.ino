
/* * * * * * * * * * * * * * * * * * * *
 *  Synth Sunrise (Arduino Version)
 *  Develop by Beta
 *  Лето 2019
 * * * * * * * * * * * * * * * * * * * */


/* Includes ------------------------------------------------------------------*/

#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <iarduino_DHT.h>
#include <DS1307RTC.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

#define Version 1.8
#define DH11_pin 7
#define LCD_I2C_Address 0x3F
#define DS1307_Address 0x68
#define BL_lcd 11
#define Relay_pin 9

/* Private variables ---------------------------------------------------------*/

byte Sun[] =        {B00100, B10101, B01110, B10001, B10001, B01110, B10101, B00100};
byte Lamp[] =       {B01110, B10001, B10001, B10001, B01010, B01110, B01110, B00100};
byte kaplya[] =     {B00100, B00100, B01010, B01010, B10001, B10001, B10001, B01110};
byte termometer[] = {B01110, B01010, B01010, B01010, B01010, B01110, B01110, B01110};
byte Seg_Pe[] =     {B11110, B11111, B11011, B11011, B11011, B11011, B11011, B11011};
byte Seg_U[] =      {B11011, B11011, B11011, B11011, B11011, B11011, B11111, B01111};
byte Seg_I[] =      {B00011, B00011, B00011, B00011, B00011, B00011, B00011, B00001};
byte Seg_O[] =      {B11110, B11111, B11011, B11011, B11011, B11011, B11111, B01111};
byte Seg_C_[] =     {B11111, B01111, B00011, B00011, B00011, B00011, B01111, B11111};
byte Seg_C[] =      {B11111, B11110, B11000, B11000, B11000, B11000, B11110, B11111};

// Переменные для времени
int8_t Hour = 0;
int8_t Min = 0;
int8_t Sec = 0;

// Время "восхода"
int8_t sun_hour;
int8_t sun_min;

// Температура и влажность
uint8_t temp = 0;
uint8_t wet = 0;

// ШИМ + АЦП
uint8_t duty = 0;
uint8_t adc = 0;

// Строки для форматного вывода
char str1_time[6];
char str2_time[6];
char m_3[3];
char string[32];

// События управления
volatile bool encoder_left;
volatile bool encoder_right;
volatile bool btn_push;
volatile bool sunrise = false;

LiquidCrystal_I2C lcd(LCD_I2C_Address, 16, 2);  // LCD 16х2
iarduino_DHT sensor(DH11_pin);  // Указание вывода DH11

// Private function prototypes ----------------------------------------------- 

// Набор символов для экрана времени
void Time_symb_pack() {
  lcd.createChar(0, kaplya);
  lcd.createChar(1, termometer);
  lcd.createChar(2, Seg_Pe);
  lcd.createChar(3, Seg_U);
  lcd.createChar(4, Seg_I);
  lcd.createChar(5, Seg_O);
  lcd.createChar(6, Seg_C_);
  lcd.createChar(7, Seg_C);
}

// Инициализация дисплея + About
void LCD_Setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Synth Sunrise");
  lcd.setCursor(0,1);
  lcd.print("Beta        v");
  lcd.print(Version);
  
  Serial.println("Synth Sunrise (Arduino Version) v1.8\r\n");
  Serial.println("By Beta\r\n");
  Serial.println("Summer 2019\r\n");
  
  delay(4000);
  lcd.clear();
}

void PWM_Setup(void) {
  TCCR2B &= ~(1<<2)|(1<<0); TCCR2B |= (1<<1); // 4 kHz (для 1kHz: TCCR2B &= ~(1<<2); TCCR2B |= (1<<1)|(1<<0);
}

// Обработка событий
void Time_event(void) {
  if(Sec == 30) {   // Чтение температуры и влажности раз в минуту
    DH11_read();
  }

 if((Hour == sun_hour)&&(Min == sun_min)&&(Sec < 1)) {   // Если текущее время равно установленному восходу, то 
  sunrise = true;
  digitalWrite(Relay_pin, LOW); analogWrite(BL_lcd, 255);
 }

 if((sunrise == true)&&(Sec == 0)) {   // Начло "восхода"
  digitalWrite(Relay_pin, LOW); analogWrite(BL_lcd, 255);
    /*//duty += 5;                                // Добавить яркость раз в одну минуту 250/5=50 минут до полной яркости
    if(duty < 10) duty++;
    else if(duty < 20) duty += 2;
         else if(duty < 40) duty += 4;
              else if(duty < 80) duty += 8;
                   else if(duty < 240) duty += 16;
                        else if(duty < 250) { duty = 250; sunrise = false; }            // Если яркость максимальная (250) то восход завершен
    analogWrite(BL_lcd, duty); */
  }

  if((Hour == (sun_hour + 2))&&(Min == sun_min)) { digitalWrite(Relay_pin, HIGH); analogWrite(BL_lcd, 0); /*duty = 0; analogWrite(BL_lcd, duty);*/ }   // Автоотключение подсветки после двух часов с начала восхода
  
}

bool send_RTC_time(uint8_t RTC_h, uint8_t RTC_m) {
  Wire.beginTransmission(DS1307_Address);
  Wire.write(0x00); // reset register pointer  
  Wire.write(0x80); // Stop the clock. The seconds will be written last
  Wire.write(RTC_m = ((RTC_m/10)<<4)|(RTC_m%10));
  Wire.write(RTC_h = ((RTC_h/10)<<4)|(RTC_h%10));
  if (Wire.endTransmission() != 0) {
    return false;
  }

  Wire.beginTransmission(DS1307_Address);
  Wire.write(0x00); // reset register pointer  
  Wire.write(0x00); // write the seconds, with the stop bit clear to restart
  if (Wire.endTransmission() != 0) {
    return false;
  }
}

void set_big_num(uint8_t num, uint8_t pos) {
  switch(num) {
    case 0: str1_time[pos] = 2; str2_time[pos] = 3; break;
    case 1: str1_time[pos] = 4; str2_time[pos] = 4; break;
    case 2: str1_time[pos] = 6; str2_time[pos] = 7; break;
    case 3: str1_time[pos] = 6; str2_time[pos] = 6; break;
    case 4: str1_time[pos] = 3; str2_time[pos] = 4; break;
    case 5: str1_time[pos] = 7; str2_time[pos] = 6; break;
    case 6: str1_time[pos] = 7; str2_time[pos] = 3; break;
    case 7: str1_time[pos] = 6; str2_time[pos] = 4; break;
    case 8: str1_time[pos] = 5; str2_time[pos] = 5; break;
    case 9: str1_time[pos] = 2; str2_time[pos] = 6; break;
  }
}

void Calk_time(void) {
  uint8_t var1 = Hour;
  set_big_num(var1%10, 1);
  var1 /= 10;
  set_big_num(var1%10, 0);
  str1_time[2] = str2_time[2] = 0xA5;
  var1 = Min;
  set_big_num(var1%10, 4);
  var1 /= 10;
  set_big_num(var1%10, 3);  
}

void Time_page(void) {
  Calk_time();
  lcd.setCursor(0,0);
  lcd.print(str1_time);
  
  sprintf(m_3, "%02d", Sec);
  lcd.print(m_3);
  
  lcd.setCursor(0,1);
  lcd.print(str2_time);
  
  if((Sec%10) <= 5) Led_block();
  if((Sec%10) >= 6) Temp_block();
}

void Led_block(void) {
  lcd.createChar(0, Sun);
  lcd.createChar(1, Lamp);
  
  lcd.setCursor(9,0);
  lcd.write(0x00); lcd.print(" ");
  sprintf(m_3, "%02d", sun_hour); lcd.print(m_3);
  lcd.print(":");
  sprintf(m_3, "%02d", sun_min); lcd.print(m_3);
  lcd.setCursor(10,1);
  lcd.print(" \x01");
  sprintf(m_3, "%3d", duty * 0.4); lcd.print(m_3);
  lcd.print("%");
}

void Temp_block(void) {
  lcd.createChar(0, kaplya);
  lcd.createChar(1, termometer);
  
  lcd.setCursor(7,0);
  lcd.print("   \x01 ");
  lcd.print(temp);
  lcd.print("\xDF""C");

  lcd.setCursor(7,1);
  lcd.print("   "); lcd.write(0x00); lcd.print(" ");
  lcd.print(wet);
  lcd.print(" %");
}

void Set_time(void) {
  // ________________________________________________ TIME
  lcd.clear();
  lcd.print("Time   [");
  sprintf(m_3, "%02d", Hour); lcd.print(m_3);
  lcd.print(":");
  sprintf(m_3, "%02d", Min); lcd.print(m_3);
  lcd.print("]");
  lcd.setCursor(0,1);
  lcd.print("set");
  lcd.setCursor(8,1);
  lcd.print("^^");
  
  while(btn_push == false) {
    lcd.setCursor(8,0);
    sprintf(m_3, "%02d", Hour); lcd.print(m_3);
    if(encoder_right == true) { Hour++; encoder_right = false; }
    if(encoder_left == true) { Hour--; encoder_left = false; }
    if(Hour >= 24) Hour = 0;
    if(Hour < 0) Hour = 24;
    delay(100);
  }
  btn_push = false;
  
  lcd.setCursor(8,1);
  lcd.print("   ^^");
  while(btn_push == false) {
    lcd.setCursor(11,0);
    sprintf(m_3, "%02d", Min); lcd.print(m_3);
    if(encoder_right == true) { Min++; encoder_right = false; }
    if(encoder_left == true) { Min--; encoder_left = false; }
    if(Min >= 60) Min = 0;
    if(Min < 0) Min = 59;
    delay(100);
  }
  btn_push = false;
  Sec = 0;

  if (send_RTC_time(Hour, Min) == true) {
    lcd.setCursor(6,1);
    lcd.print("I2C Error!");
  }
    else {
      lcd.setCursor(11,1);
      lcd.print("Done!");
    }
  delay(1000);
  lcd.clear();
  btn_push = false;
}

void Set_sunrise_time(void) {
  lcd.clear();
  lcd.print("Sunrise [");
  sprintf(m_3, "%02d", sun_hour); lcd.print(m_3);
  lcd.print(":");
  sprintf(m_3, "%02d", sun_min); lcd.print(m_3);
  lcd.print("]");
  lcd.setCursor(1,1);
  lcd.print("time");
  lcd.setCursor(9,1);
  lcd.print("^^");

  while(btn_push == false) {
    lcd.setCursor(9,0);
    sprintf(m_3, "%02d", sun_hour); lcd.print(m_3);
    if(encoder_right == true) { sun_hour++; encoder_right = false; }
    if(encoder_left == true) { sun_hour--; encoder_left = false; }
    if(sun_hour >= 24) sun_hour = 0;
    if(sun_hour < 0) sun_hour = 24;
    delay(100);
  }
  btn_push = false;
  
  lcd.setCursor(9,1);
  lcd.print("   ^^");
  while(btn_push == false) {
    lcd.setCursor(12,0);
    sprintf(m_3, "%02d", sun_min); lcd.print(m_3);
    if(encoder_right == true) { sun_min++; encoder_right = false; }
    if(encoder_left == true) { sun_min--; encoder_left = false; }
    if(sun_min >= 60) sun_min = 0;
    if(sun_min < 0) sun_min = 59;
    delay(100);
  }

  EEPROM.update(0, sun_hour);
  EEPROM.update(1, sun_min);
  lcd.setCursor(11,1);
  lcd.print("Done!");
  delay(1000);
  lcd.clear();
  btn_push = false;
}

void Menu(void) {
  btn_push = false;
  lcd.clear();
  lcd.print("  Clock set");
  lcd.setCursor(2,1);
  lcd.print("Sunrise time");
  encoder_left = true;

  while(btn_push == false) {
    delay(100);
    if(encoder_left == true) {
      //encoder_right = false;
      lcd.setCursor(0,0);
      lcd.write(0x7E);
      lcd.setCursor(0,1);
      lcd.write(0x20);
    }
    if(encoder_right == true) {
      //encoder_left = false;
      lcd.setCursor(0,0);
      lcd.write(0x20);
      lcd.setCursor(0,1);
      lcd.write(0x7E);
    }
  }
  if(encoder_left == true) { btn_push = encoder_left = false; Set_time(); }
  if(encoder_right == true) { btn_push = encoder_right = false; Set_sunrise_time(); }
}

void Off_sunrise(void) {
  digitalWrite(2, HIGH);
  sunrise = false;
  duty = 0;
  analogWrite(BL_lcd, duty);
  delay(100);
  btn_push = false;
}

void Check_encoder(void) {
  if(encoder_left == true && sunrise == true) { Off_sunrise(); encoder_left = false; }
  if(encoder_right == true && sunrise == false) { digitalWrite(Relay_pin, LOW); analogWrite(BL_lcd, 255); /*duty > 240 ? duty = 250 : duty += 10; analogWrite(BL_lcd, duty);*/ encoder_right = false; }
  if(encoder_left == true && sunrise == false) { digitalWrite(Relay_pin, HIGH); analogWrite(BL_lcd, 0);/*duty = 0; analogWrite(BL_lcd, duty);*/ encoder_left = false; }
  if(btn_push == true) { Menu(); }
}

void DH11_read(void) {
  sensor.read();
  temp = sensor.tem;
  wet = sensor.hum;
}

void RTC_read(void) {
  tmElements_t tm;
  RTC.read(tm);
  Sec = tm.Second;
  Min = tm.Minute;
  Hour = tm.Hour;
}

void LCD_bright(void) {
  adc = analogRead(A0)>>2;
  if(adc == 0) adc = 1;
  analogWrite(5, adc);
}

void UART_debug_info(void) {
  sprintf(string, "Time [%02d:%02d:%02d]", Hour, Min, Sec);
  Serial.println(string);
  sprintf(string, "Sunrise %02d:%02d", sun_hour, sun_min);
  Serial.println(string);
  sprintf(string, "Temp: %d'C  Hum: %d%%", temp, wet);
  Serial.println(string);
  sprintf(string, "Duty = %d   ADC = %d", duty, adc);
  Serial.println(string);
  Serial.println(" ");
}

// User code begin ----------------------------------------------------

void setup() {
  pinMode(13, OUTPUT);    //LED13
  pinMode(4, INPUT);      //encoder pin 1
  pinMode(Relay_pin, OUTPUT);      //relay pin 9
  attachInterrupt(0, encoder_rotate, FALLING);  //encoder pin 2
  attachInterrupt(1, push_enc_btn, FALLING);    //encoder button
  
  Serial.begin(9600);
  
  PWM_Setup();
  LCD_Setup();
  Time_symb_pack();
  sun_hour = EEPROM.read(0);
  sun_min = EEPROM.read(1);
  DH11_read();
}


void loop() {  
  //digitalWrite(13, HIGH);
  RTC_read();
  Time_page();
  Check_encoder();
  //digitalWrite(13, LOW);
  delay(228);
  Check_encoder();
  delay(200);
  
  //digitalWrite(13, HIGH);
  lcd.setCursor(2,0);
  lcd.printByte(0x20);
  lcd.setCursor(2,1);
  lcd.printByte(0x20);
  
  Time_event();
  LCD_bright();
  Check_encoder();
  //UART_debug_info();
  //digitalWrite(13, LOW);
  delay(275);
  Check_encoder();
  delay(200);
}

// Прерывания
void encoder_rotate() {
  if((digitalRead(4)) == 0) { encoder_left = true; encoder_right = false; }
    else { encoder_right = true; encoder_left = false; }
  for(uint8_t i=0; i<255; i++);
}

void push_enc_btn() {
  for(uint8_t i=0; i<255; i++);
  if((digitalRead(3)) == 0) btn_push = true;
}



/*
 * Дата
 // Переменные для даты
int8_t Day = 1;
int8_t Date = 1;
int8_t Month = 1;
int8_t Year = 0;

void Print_month(void) {
  switch(Month) {
    case 1: { lcd.print("Jan."); break; }
    case 2: { lcd.print("Feb."); break; }
    case 3: { lcd.print("Mar."); break; }
    case 4: { lcd.print("Apr."); break; }
    case 5: { lcd.print("May "); break; }
    case 6: { lcd.print("June"); break; }
    case 7: { lcd.print("July"); break; }
    case 8: { lcd.print("Aug."); break; }
    case 9: { lcd.print("Sept"); break; }
    case 10: { lcd.print("Oct."); break; }
    case 11: { lcd.print("Nov."); break; }
    case 12: { lcd.print("Dec."); break; }
  }
}

void Print_Wday(void) {
  switch(Day) {
    case 1: { lcd.print("Mon.[1/7]"); break; }
    case 2: { lcd.print("Tue.[2/7]"); break; }
    case 3: { lcd.print("Wed.[3/7]"); break; }
    case 4: { lcd.print("Thu.[4/7]"); break; }
    case 5: { lcd.print("Fri.[5/7]"); break; }
    case 6: { lcd.print("Sat.[6/7]"); break; }
    case 7: { lcd.print("Sun.[7/7]"); break; }
  }
}

 if((Sec%10) <= 5) Date_block();

 void Date_block(void) {
  lcd.setCursor(7,0);
  lcd.print("  ");
  sprintf(m_3, "%2d", Date);
  lcd.print(m_3);
  lcd.setCursor(12,0);
  Print_month();
  lcd.setCursor(7,1);
  Print_Wday();
}

Day = tm.Wday;
  Date = tm.Day;
  Month = tm.Month;
  Year = tm.Year;

 //Установка даты
  lcd.clear();
  lcd.print("Date[");
  sprintf(m_3, "%02d", Date); lcd.print(m_3);
  lcd.print(".");
  sprintf(m_3, "%02d", Month); lcd.print(m_3);
  lcd.print(".20");
  sprintf(m_3, "%02d", Year); lcd.print(m_3);
  lcd.print("]");
  lcd.setCursor(0,1);
  lcd.print("set");
  lcd.setCursor(5,1);
  lcd.print("^^");
  
  while(btn_push == false) {
    lcd.setCursor(5,0);
    sprintf(m_3, "%02d", Date); lcd.print(m_3);
    if(encoder_left == true) { Date++; encoder_left = false; }
    if(encoder_right == true) { Date--; encoder_right = false; }
    if(Date > 31) Date = 1;
    if(Date < 1) Date = 31;
    delay(100);
  }
  btn_push = false;
  
  lcd.setCursor(5,1);
  lcd.print("   ^^");
  while(btn_push == false) {
    lcd.setCursor(8,0);
    sprintf(m_3, "%02d", Month); lcd.print(m_3);
    if(encoder_left == true) { Month++; encoder_left = false; }
    if(encoder_right == true) { Month--; encoder_right = false; }
    if(Month > 12) Month = 1;
    if(Month < 1) Month = 12;
    delay(100);
  }
  btn_push = false;

  lcd.setCursor(8,1);
  lcd.print("     ^^");
  while(btn_push == false) {
    lcd.setCursor(13,0);
    sprintf(m_3, "%02d", Year); lcd.print(m_3);
    if(encoder_left == true) { Year++; encoder_left = false; }
    if(encoder_right == true) { Year--; encoder_right = false; }
    if(Year > 99) Year = 0;
    if(Year < 0) Year = 99;
    delay(100);
  }

// Функции отрисовки графика (вырезано, ибо юзлес)

// Набор символов для экрана графика
void Row_symb_pack() {
  lcd.createChar(1, row1);
  lcd.createChar(2, row2);
  lcd.createChar(3, row3);
  lcd.createChar(4, row4);
  lcd.createChar(5, row5);
  lcd.createChar(6, row6);
  lcd.createChar(7, row7);
}

// Массивы для графика температуры и влажности за 12 часов
int temp_12h[12] = {17,18,19,20,21,22,23,24,25,26,27,28};
int wet_12h[12] = {24,23,22,21,20,19,18,17,16,15,14,13};

void drawPlot(byte pos, byte row, byte width, byte height, int min_val, int max_val, int *plot_array) {
  for (byte i = 0; i < width; i++) {                  // каждый столбец параметров
    int fill_val = plot_array[i];
    fill_val = constrain(fill_val, min_val, max_val);
    byte infill, fract;
    // найти количество целых блоков с учётом минимума и максимума для отображения на графике
    infill = floor((float)(plot_array[i] - min_val) / (max_val - min_val) * height * 10);
    fract = (infill % 10) * 8 / 10;                   // найти количество оставшихся полосок
    infill = infill / 10;

    for (byte n = 0; n < height; n++) {     // для всех строк графика
      if (n < infill && infill > 0) {       // пока мы ниже уровня
        lcd.setCursor((i + pos), (row - n));        // заполняем полными ячейками
        lcd.printByte(0xFF);
      }
      if (n >= infill) {                    // если достигли уровня
        lcd.setCursor((i + pos), (row - n));
        if (fract > 0) lcd.write(fract);          // заполняем дробные ячейки
        else lcd.write(16);                       // если дробные == 0, заливаем пустой
        for (byte k = n + 1; k < height; k++) {   // всё что сверху заливаем пустыми
          lcd.setCursor((i + pos), (row - k));
          lcd.write(16);
        }
        break;
      }
    }
  }
}

void Temp_graph(void) {
  lcd.clear();
  lcd.createChar(0, termometer);
  Row_symb_pack();
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.setCursor(13,0);
  lcd.print("\xA2""30");
  lcd.setCursor(13,1);
  lcd.print("\xA3""15");
  drawPlot(1, 1, 12, 2, 15, 30, (int*)temp_12h);
  delay(5000);
  lcd.clear();
  Time_symb_pack();
  
}

void Wet_graph(void) {
  lcd.clear();
  lcd.createChar(0, kaplya);
  Row_symb_pack();
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.setCursor(13,0);
  lcd.print("\xA2""25");
  lcd.setCursor(13,1);
  lcd.print("\xA3""10");
  drawPlot(1, 1, 12, 2, 10, 25, (int*)wet_12h);
  delay(5000);
  lcd.clear();
  Time_symb_pack();
}

  if((Hour == 0)&&(Min == 0)&&(Sec == 0)) {      
    for(uint8_t i=1; i<=11; i++) {
      temp_12h[i-1] = temp_12h[i]; 
    }
    temp_12h[11] = temp;
      
    for(uint8_t i=1; i<=11; i++) {
      wet_12h[i-1] = wet_12h[i]; 
    }
    wet_12h[11] = wet;
  }

*/
