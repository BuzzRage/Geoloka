#include <Arduino.h>
#include "include/global.h"

float tension = 0;
int curr_btn = 0;
bool autowrite = false;
int write_freq = 1000;
int buff = 0;
int nbpts = -1;

enum mode current_mode = batterie;

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

  countLine();
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
      erase_EEPROM_datas();
      break;
  }

  if(autowrite){
    if(buff*10 == write_freq){
      write_CSV_entry();
      float t = (((gps.time.hour()+2)%24)*60*60+gps.time.minute()*60+gps.time.second());
      float lat = gps.location.lat();
      float lng = gps.location.lng();

      if(nbpts==0){
        store_EEPROM_data(ADDR_TPS0,t);
        store_EEPROM_data(ADDR_LAT0,lat);
        store_EEPROM_data(ADDR_LNG0,lng);
      }
      else{
        update_route_data(lat, lng, t);
      }
      buff = 0;
    }
    else
      buff++;
  }
  display(current_mode);
  runGPS();

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

void update_route_data(float lat, float lng, float t){

  float dx    = 0;
  float lat0  = 0;
  float lng0  = 0;

  float dt    = 0;
  float t0    = 0;

  float m_vit = 0;
  float v     = 0;

  load_EEPROM_data(ADDR_DST,&dx);
  load_EEPROM_data(ADDR_LAT0,&lat0);
  load_EEPROM_data(ADDR_LNG0,&lng0);

  load_EEPROM_data(ADDR_TPS,&dt);
  load_EEPROM_data(ADDR_TPS0,&t0);

  load_EEPROM_data(ADDR_VIT,&m_vit);

  dx += (gps.distanceBetween(lat0,lng0,lat,lng)/6372795.0)*RAYON_TERRE;
  dt += t-t0;
  v  = (gps.distanceBetween(lat0,lng0,lat,lng)/6372795.0)*RAYON_TERRE/(t-t0);
  m_vit = m_vit + (v-m_vit)/nbpts;

  store_EEPROM_data(ADDR_DST,dx);
  store_EEPROM_data(ADDR_LAT0,lat);
  store_EEPROM_data(ADDR_LNG0,lng);

  store_EEPROM_data(ADDR_TPS,dt);
  store_EEPROM_data(ADDR_TPS0,t);
  store_EEPROM_data(ADDR_VIT,m_vit);
}
