#ifndef MEMORY_H
#define MEMORY_H

#define SD_SCK                13
#define SD_MISO               12
#define SD_MOSI               11
#define SD_CS                 10

#define FILENAME							"gpslog.csv"

const char PROGMEM csv_header[] =	{"Date,Heure,Latitude,Longitude,Altitude,NBsat,HDOP"};

int countLine();

void write_CSV_entry();
void upload_CSV_file();
void erase_file();

float load_EEPROM_data(uint8_t addr);
void store_EEPROM_data(uint8_t addr, float value);

#endif
