#include "include/global.h"

File	dataFile;

int countLine(){
  dataFile = SD.open(FILENAME, FILE_READ);
  if(dataFile){
    nbpts = -1;
    while(dataFile.available()){
      if(dataFile.read() == '\r'){
        nbpts++;
      }
    }
    dataFile.close();
  }
  else
    DISPLAY_PRINTLN(F("Can't open "+String(FILENAME)));

  return nbpts;
}

float load_EEPROM_data(uint8_t addr){
  return EEPROM.read(addr);
}

void store_EEPROM_data(uint8_t addr, float value){
  EEPROM.write(addr, value);
}

void write_CSV_entry(){

  if(!SD.exists(FILENAME)){
    dataFile = SD.open(FILENAME, FILE_WRITE);
    for (byte k = 0; k < strlen_P(csv_header); k++) {
      dataFile.write(pgm_read_byte_near(csv_header + k));
    }
    dataFile.println();
    DISPLAY_PRINTLN(F(String(FILENAME) + " created"));
    nbpts = 0;
  }
  else
    dataFile = SD.open(FILENAME, FILE_WRITE);

  if(dataFile){
    String dataString = "";
    dataString += (gps.date.isValid() ? String(gps.date.day())+"/"+String(gps.date.month())+"/"+String(gps.date.year()) + ",": "null,");

    if(gps.time.isValid()){
      int heure = (gps.time.hour()+2)%24;
      if (heure < 10)                 dataString += "0";
      dataString += String(heure) + ":";
      if (gps.time.minute() < 10)     dataString += "0";
      dataString += String(gps.time.minute()) + ":";
      if (gps.time.second() < 10)     dataString += "0";
      dataString += String(gps.time.second());
    }
    else{
      dataString += "null";
    }

    String lat = String(gps.location.lat(),6);
    String lng = String(gps.location.lng(),6);

    if(gps.location.isValid())
      dataString += ","+lat+","+lng;
    else
      dataString += ",null";
    dataString += ","+String(gps.altitude.meters());

    DISPLAY_PRINT(F(String(FILENAME) + " will be written with:  "));
    DISPLAY_PRINTLN(F(dataString));

    dataFile.println(dataString);
    dataFile.close();

    nbpts++;
    DISPLAY_PRINTLN(F(String(FILENAME) + " wrotten"));
  }
  else
    DISPLAY_PRINTLN(F("Error opening "+String(FILENAME)));
}

void upload_CSV_file(){
  dataFile = SD.open(FILENAME, FILE_READ);
  if(dataFile){
    DISPLAY_PRINTLN(F(String(FILENAME)+" will be read"));

    while(dataFile.available())
      Serial.write(dataFile.read());
    dataFile.close();

    DISPLAY_PRINTLN(F(String(FILENAME)+" read"));
  }
  else
    DISPLAY_PRINTLN(F("Can't open "+String(FILENAME)));
}

void erase_file(){
  if(SD.exists(FILENAME)){
    DISPLAY_PRINTLN(F(String(FILENAME)+" will be removed"));
    if(SD.remove(FILENAME))
      DISPLAY_PRINTLN(F(String(FILENAME)+" removed"));
    else
      DISPLAY_PRINTLN(F("Impossible to remove " + String(FILENAME)));
  }
  else{
    DISPLAY_PRINTLN(F(String(FILENAME)+" does not exists"));
  }
}
