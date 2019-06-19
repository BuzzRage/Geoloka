#ifndef MEMORY_H
#define MEMORY_H

#define SD_SCK                13
#define SD_MISO               12
#define SD_MOSI               11
#define SD_CS                 10

#define FILENAME							"gpslog.csv"

const char PROGMEM csv_header[] =	{"Date,Heure,Latitude,Longitude,Altitude,NBsat,HDOP"};

#define ADDR_DST              0x08
#define ADDR_TPS              0x0C
#define ADDR_VIT              0x14
#define ADDR_TPS0             0x18
//#define ADDR_DAY0             0x
#define ADDR_LAT0             0x20
#define ADDR_LNG0             0x24

int countLine();

void write_CSV_entry();
void upload_CSV_file();
void erase_file();

float load_EEPROM_data(uint8_t addr, float *value);
void store_EEPROM_data(uint8_t addr, float value);
void erase_EEPROM_datas();

#endif
