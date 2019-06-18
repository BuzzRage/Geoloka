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

/* ------- Defines peripheral pins ------- */


#define SD_SCK                13
#define SD_MISO               12
#define SD_MOSI               11
#define SD_CS                 10

#define FILENAME							"gpslog.csv"
const char PROGMEM csv_header[] =	{"Date,Heure,Latitude,Longitude"};

const char PROGMEM s_slash[]	 = "/";
const char PROGMEM s_zero[]		 = "0";
const char PROGMEM s_sep[]		 = ":";

const char PROGMEM s_mode[] 	 = "Mode:";
const char PROGMEM s_auto[]		 = "AUTO  ";
const char PROGMEM s_manuel[]	 = "MANUEL";

const char PROGMEM s_v[]			 = "V: ";
const char PROGMEM s_hleft[]	 = "H Left";

const char PROGMEM s_process[] = "Process.";
const char PROGMEM s_alti[]		 = "Altitude";
const char PROGMEM s_m[]			 = "m";
const char PROGMEM s_hdop[]		 = "HDOP:";
const char PROGMEM s_nbsat[]	 = "NB SAT:";
const char PROGMEM s_err[]		 = "Error";
const char *const lcd_strings[] PROGMEM = {
	s_slash,
	s_zero,
	s_sep,
	s_mode,
	s_auto,
	s_manuel,
	s_v,
	s_hleft,
	s_process,
	s_alti,
	s_m,
	s_hdop,
	s_nbsat,
	s_err
};

#define VBAT_PIN              14


float getTension();
float getAutonomy(float t);

void load_EEPROM_data();
void store_EEPROM_data();
void update_route_data();

void write_CSV_entry();
void upload_CSV_file();
void erase_file();

extern TinyGPSPlus gps;
extern SoftwareSerial ss;
extern LiquidCrystal lcd;
extern Bounce debouncerBP0;
extern Bounce debouncerBP1;
extern Bounce debouncerBPEN;
extern Bounce btn[3];
extern bool autowrite;

#endif
