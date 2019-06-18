#ifndef LCD_H
#define LCD_H

#define LCD_RS                4
#define LCD_EN                5
#define LCD_D4                6
#define LCD_D5                7
#define LCD_D6                8
#define LCD_D7                9

#define LCD_COLS							8
#define LCD_ROWS							2

#define NB_MODES	6
enum mode { menu, wmode, batterie, coordonnees, altitude, hdop, nbsat};

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

void display(mode m);

#endif
