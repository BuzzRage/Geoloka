#ifndef GLOBAL_H
#define GLOBAL_H

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Bounce2.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

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

//#define DEBUG_GPS
#ifdef DEBUG_GPS
	void displayInfo();
#endif

#define SERIAL_SPEED 		      9600

/* ------- Defines peripheral pins ------- */
#define BPEN                  17
#define BP0                   16
#define BP1                   15

#define SD_SCK                13
#define SD_MISO               12
#define SD_MOSI               11
#define SD_CS                 10

#define FILENAME							"gpslog.csv"
const char PROGMEM csv_header[] =	{"Date,Heure,Latitude,Longitude"};

#define LCD_RS                4
#define LCD_EN                5
#define LCD_D4                6
#define LCD_D5                7
#define LCD_D6                8
#define LCD_D7                9

#define LCD_COLS							8
#define LCD_ROWS							2

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

#define GPS_RX								3
#define GPS_TX								2
#define GPS_BAUD							4800

#define VBAT_PIN              14


#define NB_MODES	6
enum mode { menu, wmode, batterie, coordonnees, altitude, hdop, nbsat};

void display(mode m);
int getBtn();
float getTension();

void initGPS();
void testGPS();

void write_CSV_entry();
void upload_CSV_file();
void erase_file();

#endif
