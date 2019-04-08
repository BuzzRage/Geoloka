#ifndef GLOBAL_H
#define GLOBAL_H

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define DEBUG_DISPLAY
#ifdef DEBUG_DISPLAY
	#define DISPLAY_PRINTLN(a) 	Serial.println(a)
	#define DISPLAY_PRINT(a) 		Serial.print(a)
	#define DISPLAY_WRITE(a) 		Serial.write(a)
#else
	#define DISPLAY_PRINTLN(a)  lcd.println(a);  // Peut-être pas pertinent
	#define DISPLAY_PRINT(a)    lcd.print(a);    // Peut-être pas pertinent
	#define DISPLAY_WRITE(a)
#endif

#define SERIAL_SPEED 		      9600

// Defines peripheral pins
#define BPEN                  16
#define BP0                   15
#define BP1                   17

#define SD_SCK                13
#define SD_MISO               12
#define SD_MOSI               11
#define SD_CS                 10

#define LCD_RS                4
#define LCD_EN                5
#define LCD_D4                6
#define LCD_D5                7
#define LCD_D6                8
#define LCD_D7                9

#define LCD_COLS							8
#define LCD_ROWS							2

#define VBAT_PIN              14

int btn[3] = {BPEN, BP0, BP1};
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

int getBtn();
float getTension();

#endif
