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

  Serial.println(countLine());
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
      update_route_data(0, (gps.time.value()/100), 0);
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

void update_route_data(float dst, float tps, float vit){
  // Faire plan mémoire + sizeof(float) = 4 octets
  uint8_t addr_dst = 0x08;
  uint8_t addr_tps = 0x0C;
  uint8_t addr_vit = 0x10;

  //float m_dst = load_EEPROM_data(addr_dst);
  //float m_tps = load_EEPROM_data(addr_tps);
  //float m_vit = load_EEPROM_data(addr_vit);

  //m_tps = m_tps + (tps-m_tps)/nbpts;

  Serial.println(nbpts);

  //store_EEPROM_data(addr_dst,m_dst);
  //store_EEPROM_data(addr_tps,m_tps);
  //store_EEPROM_data(addr_vit,m_vit);
}
