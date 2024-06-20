#include "TFT.h"
#include "SPI.h"
#include "DHT.h"
#include <GyverHTU21D.h>
#include <GyverBME280.h> 


#define cs 10  // пин для подключения CS
#define dc 9   // пин для подключения A0
#define rst 8  // пин для подключения reset

#define DHTPIN_1 2
#define DHTPIN_2 3
#define DHTTYPE DHT11

char lcd_buf[20];

DHT dht1(DHTPIN_1, DHTTYPE);
DHT dht2(DHTPIN_2, DHTTYPE);
GyverHTU21D htu;
GyverBME280 bme; 
TFT screen = TFT(cs, dc, rst);

void read_dht1(){
  float h = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht1.readTemperature();
  if (isnan(h) || isnan(t)) {
    screen.text("Failed DHT #1 sensor!", 5, 100);
    return;
  }
  sprintf(lcd_buf, "%02d`C    %02d%%", (int)t, (int)h);
  screen.text(lcd_buf, 70, 25);
  //screen.text(lcd_buf, 70, 40);
}

void read_dht2(){
  float h = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht2.readTemperature();
  if (isnan(h) || isnan(t)) {
    screen.text("Failed DHT #2 sensor!", 5, 110);
    return;
  }
  sprintf(lcd_buf, "%02d`C    %02d%%", (int)t, (int)h);
  screen.text(lcd_buf, 70, 40);
}

void read_htu21(){
  int t, h;
  htu.requestTemperature();
  delay(100);
  if (htu.readTemperature()) {              // Читаем температуру из датчика и проверяем
    t = (int)htu.getTemperature();
  }
  htu.requestHumidity(); 
  delay(100);
  if (htu.readHumidity()) {                 // Читаем влажность из датчика и проверяем
    h = (int)htu.getHumidity();
  }
  sprintf(lcd_buf, "%02d`C    %02d%%", t, h);
  screen.text(lcd_buf, 70, 70);
}

void read_bme280(){
  int t, h;
  t = (int)bme.readTemperature();
  h = (int)bme.readHumidity();
  sprintf(lcd_buf, "%02d`C    %02d%%", t, h);
  screen.text(lcd_buf, 70, 55);
}

void setup() {
  Serial.begin(19200); 
  dht1.begin();
  dht2.begin();
  if (!htu.begin()) screen.text("Failed HTU21D sensor!", 5, 120);
  bme.begin();

  screen.begin();
  screen.background(255, 255, 255);
  screen.setTextSize(1);
  screen.stroke(0, 0, 0);
  screen.fill(255, 255, 255);

  screen.text("SENSOR    Temp    Hum", 10, 10);
  screen.text("DHT11", 10, 25);
  screen.text("DHT11", 10, 40);
  screen.text("BME280", 10, 55);
  screen.text("HTU21D", 10, 70);


}

void loop() {
  read_htu21();
  read_dht1();
  read_dht2();
    read_bme280();
  delay(2000);
  screen.rect(65, 20, 30, 60);
  screen.rect(115, 20, 30, 60);
}