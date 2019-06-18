#include <Arduino.h>
#include "include/global.h"

float tension = 0;
int curr_btn = 0;
bool autowrite = false;
int write_freq = 1000;
int buff = 0;


enum mode current_mode = batterie;
File dataFile;

void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(VBAT_PIN, INPUT);

  pinMode(BP0,  INPUT_PULLUP);
  pinMode(BP1,  INPUT_PULLUP);
  pinMode(BPEN, INPUT_PULLUP);

  lcd.begin(LCD_COLS, LCD_ROWS);

  DISPLAY_PRINTLN(F("Initializing SD card..."));

  if(!SD.begin(SD_CS)){
    DISPLAY_PRINTLN(F("Card failed, or not present"));
    while (1);
  }

  DISPLAY_PRINTLN(F("Card initialized."));

  initGPS();
}

void loop(){
  curr_btn = getBtn();
  switch(curr_btn){
    case 1:
      if(current_mode < NB_MODES)
        current_mode = (mode)current_mode + 1;
      else
        current_mode = 0;
      lcd.clear();
      break;
    case 2:
      if(current_mode == wmode)
        autowrite = !autowrite;
      else
        write_CSV_entry();
      break;
    case 3:
      upload_CSV_file();
      break;
    case 4:
      erase_file();
      break;
  }

  if(autowrite){
    if(buff*10 == write_freq){
      write_CSV_entry();
      buff = 0;
    }
    else
      buff++;
  }
  display(current_mode);
  testGPS();

  delay(10);
}

float getTension(){
  return (float(analogRead(VBAT_PIN))/1023.0)*6.0;
}

float getAutonomy(float t){
  if(t>4 && t<6.2){
    float trefs[21] = {6.2000,6.0900,5.9800,5.8700,5.7600,5.6500,5.5400,5.4300,5.3200,5.2100,5.1000,4.9900,4.8800,4.7700,4.6600,4.5500,4.4400,4.3300,4.2200,4.1100,4.0000};
    int i=0, hoursleft = -1;
    for(i=0;i<21;i++){
      if(i+1<21 && t<trefs[i] && t>trefs[i+1]){
        hoursleft = 21-i;
      }
      else if(i+1 == 21 && t<trefs[i-1] && t>trefs[i])
        hoursleft = 21-i;
    }
    return hoursleft;
  }
  return -2;
}

void load_EEPROM_data(){
    int test;
    EEPROM.get(0, test);
    Serial.print(test);
}

void store_EEPROM_data(){

}

void update_route_data(){

}

void write_CSV_entry(){

  if(!SD.exists(FILENAME)){
    dataFile = SD.open(FILENAME, FILE_WRITE);
    dataFile.println(csv_header);
    DISPLAY_PRINTLN(F(String(FILENAME) + " created"));
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

    DISPLAY_PRINT(F(String(FILENAME) + " will be written with:  "));
    DISPLAY_PRINTLN(F(dataString));

    dataFile.println(dataString);
    dataFile.close();

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
