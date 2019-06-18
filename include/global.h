#ifndef GLOBAL_H
#define GLOBAL_H

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Bounce2.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

#include "gps.h"
#include "lcd.h"
#include "btn.h"
#include "memory.h"

//#define DEBUG_DISPLAY
#ifdef DEBUG_DISPLAY
	#define DISPLAY_PRINTLN(a) 	Serial.println(a)
	#define DISPLAY_PRINT(a) 		Serial.print(a)
	#define DISPLAY_WRITE(a) 		Serial.write(a)
#else
	#define DISPLAY_PRINTLN(a)
	#define DISPLAY_PRINT(a)
	#define DISPLAY_WRITE(a)
#endif

#define SERIAL_SPEED 		      9600
#define VBAT_PIN              14


float getTension();
float getAutonomy(float t);
void update_route_data(float dst, float tps, float vit);


extern File 					dataFile;
extern TinyGPSPlus 		gps;
extern SoftwareSerial ss;
extern LiquidCrystal 	lcd;

extern Bounce debouncerBP0;
extern Bounce debouncerBP1;
extern Bounce debouncerBPEN;
extern Bounce btn[3];

extern bool autowrite;
extern int nbpts;

#endif
