#ifndef MEMORY_H
#define MEMORY_H

#define SD_SCK                13
#define SD_MISO               12
#define SD_MOSI               11
#define SD_CS                 10

#define FILENAME							 "gpslog.csv"

const char PROGMEM csv_header[] =	{"Date,Heure,Latitude,Longitude"};

void write_CSV_entry();
void upload_CSV_file();
void erase_file();

void load_EEPROM_data();
void store_EEPROM_data();

#endif
