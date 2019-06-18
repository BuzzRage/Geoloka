#include <Arduino.h>
#include "include/global.h"

float tension = 0;
int btnEn = 0;
bool autowrite = false;
int write_freq = 1000;
int buff = 0;

Bounce debouncerBP0  = Bounce(BP0,5);
Bounce debouncerBP1  = Bounce(BP1,5);
Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce btn[3] = {debouncerBP0,debouncerBP1,debouncerBPEN};

enum mode current_mode = batterie;
File dataFile;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
SoftwareSerial ss(GPS_RX, GPS_TX);
TinyGPSPlus gps;


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
  btnEn = getBtn();
  switch(btnEn){
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

int getBtn(){
  bool btnVal[2] = {false,false};
  btn[0].update();
  btn[1].update();
  btn[2].update();

  if(btn[2].rose()){
    btnVal[0] = btn[0].read();
    btnVal[1] = btn[1].read();

    if     ( !btnVal[1] && !btnVal[0])
      return 1;
    else if( !btnVal[1] &&  btnVal[0])
      return 2;
    else if( btnVal[1]  && !btnVal[0])
      return 3;
    else if( btnVal[1]  &&  btnVal[0])
      return 4;
    else
      return -1;
  }
  else
    return 0;
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

void display(mode m){
    char buffer[8];
    if(m == menu){
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[0])));

      lcd.setCursor(0, 0);
      lcd.print(gps.date.day());
      lcd.print(buffer);
      lcd.print(gps.date.month());
      lcd.print(buffer);
      lcd.print(gps.date.year()-2000);
      lcd.setCursor(0, 1);

      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[1])));
      int heure  = (gps.time.hour()+2)%24;
      int minute = gps.time.minute();
      if(heure < 10)      lcd.print(buffer);
      lcd.print(heure);
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[2])));
      lcd.print(buffer);
      if(minute < 10)     lcd.print(buffer);
      lcd.print(minute);
    }
    else if(m == wmode){
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[3])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);

      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[4])));
      lcd.setCursor(0, 1);
      if(autowrite)
        strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[4])));
      else
        strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[5])));
      lcd.print(buffer);
    }
    else if(m == batterie){
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[6])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      float tension = getTension();
      lcd.print(tension);

      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[7])));
      int autonomy = getAutonomy(tension);
      lcd.setCursor(0, 1);
      lcd.print(autonomy);
      lcd.print(buffer);
    }
    else if(m == coordonnees){
      lcd.setCursor(0, 0);
      if(gps.location.isValid()){
        lcd.print(gps.location.lat(),6);
        lcd.setCursor(0, 1);
        lcd.print(gps.location.lng(),6);
      }
      else{
        strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[8])));
        lcd.print(buffer);
      }
    }
    else if(m == altitude){
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[9])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);

      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[10])));
      lcd.setCursor(0, 1);
      lcd.print(gps.altitude.meters());
      lcd.print(buffer);
    }
    else if(m == hdop){
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[11])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      lcd.print(gps.hdop.value());
    }
    else if(m == nbsat){
      lcd.setCursor(0, 0);
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[12])));
      lcd.print(buffer);
      lcd.setCursor(0, 1);
      lcd.print(gps.satellites.value());
    }
    else{
      strcpy_P(buffer,(char *)pgm_read_word(&(lcd_strings[13])));
      lcd.setCursor(0, 0);
      lcd.print(buffer);
    }
}

void initGPS(){
  ss.begin(GPS_BAUD);
}

void testGPS(){
  while(ss.available() > 0)
    if(gps.encode(ss.read())){
      #ifdef DEBUG_GPS
      displayInfo();
      #endif
    }

  if (millis() > 5000 && gps.charsProcessed() < 10){
    DISPLAY_PRINTLN(F("No GPS detected: check wiring."));
    while(true);
  }
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

#ifdef DEBUG_GPS

void displayInfo(){
  Serial.print(F("Location: "));
  if (gps.location.isValid()){
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
    Serial.print(F("INVALID"));

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()){
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
    Serial.print(F("INVALID"));

  Serial.print(F(" "));
  if (gps.time.isValid()){
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour()+2);
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
  }
  else
    Serial.print(F("INVALID"));

  Serial.println();
}

#endif
