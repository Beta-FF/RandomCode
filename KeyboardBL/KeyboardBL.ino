#include <SoftwareUart.h>
#include "VirtButton.h"

#define KEY_UP		    4	//PIN_PA0
#define KEY_DOWN	    5	//PIN_PA1
#define KEY_DEL		    6	//PIN_PA2
#define KEY_FN		    7	//PIN_PA3
#define PIN_BACKLIGHT   11	//PIN_PB3

#define MOD_OFF         1
#define MOD_ON          2
#define MOD_BRIGHT_UP   3
#define MOD_BRIGHT_DN   4

#define TX_PIN  14

SoftwareUart <TX_PIN> uart;

EasyButton btn_up;
EasyButton btn_down;
EasyButton btn_del;

uint8_t state = MOD_OFF;
//uint8_t brightness[] = {255, };

uint32_t t0 = 0;
uint32_t t1 = 0;
uint8_t cnt_key_up = 0;
uint8_t cnt_key_down = 0;
uint8_t cnt_key_del = 0;
uint8_t cnt_key_fn = 0;

void PWM_setup() {
	// Частота шим ~ 1 кГц
    //f_PWM = (f_PLL / (OCR1C + 1)) / div
    //f_PWM = (64 МГц / (255 + 1)) / 256 = 977 Гц
	TCCR1B = 0b1001;        // Предделитель = 256 (0b0111 = 64, 0b1000 = 128, 0b1001 = 256)
    OCR1A = OCR1B = 0x00;   // Сброс заполнения ШИМ
    OCR1C = 0xFF;           // Разрешение ШИМ 8 бит
}

void keyboard_handler() {
    if(digitalRead(KEY_UP) == LOW) cnt_key_up = 5;
    if(digitalRead(KEY_DOWN) == LOW) cnt_key_down = 5;
    if(digitalRead(KEY_DEL) == LOW) cnt_key_del = 5;
    if(digitalRead(KEY_FN) == LOW) cnt_key_fn = 5;

    if(millis() - t0 > 10) {
        t0 = millis();
        if(cnt_key_up != 0) cnt_key_up--;
        btn_up.check(cnt_key_up == 0 ? true : false);
        if(cnt_key_down != 0) cnt_key_down--;
        btn_up.check(cnt_key_down == 0 ? true : false);
        if(cnt_key_del != 0) cnt_key_del--;
        btn_up.check(cnt_key_del == 0 ? true : false);
        if(cnt_key_fn != 0) cnt_key_fn--;
    }
}


void setup() {
    pinMode(KEY_UP, INPUT_PULLUP);
    pinMode(KEY_DOWN, INPUT_PULLUP);
    pinMode(KEY_DEL, INPUT_PULLUP);
    pinMode(KEY_FN, INPUT_PULLUP);
    PWM_setup();
    analogWrite(PIN_BACKLIGHT, 255);
    //uart.printStr("OK\n");
}

void loop() {
    keyboard_handler();
    if(millis() - t1 > 5) {
        t1 = millis();
        switch(state) {
			case MOD_ON:
			    break;
			case MOD_OFF: 
                break;
			case MOD_BRIGHT_DN:
			    break;
			case MOD_BRIGHT_UP:
			    break;
		}
    }
}
