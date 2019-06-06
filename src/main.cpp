#include <Arduino.h>
#include "include/global.h"

float tension = 0;
int btnEn = 0;
Bounce debouncerBP0  = Bounce(BP0,5);
Bounce debouncerBP1  = Bounce(BP1,5);
Bounce debouncerBPEN = Bounce(BPEN,5);
Bounce btn[3] = {debouncerBP0,debouncerBP1,debouncerBPEN};

enum mode current_mode = menu;
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

  DISPLAY_PRINTLN("Initializing SD card...");

  if(!SD.begin(SD_CS)){
    DISPLAY_PRINTLN("Card failed, or not present");
    while (1);
  }

  DISPLAY_PRINTLN("Card initialized.");

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
    case 2: // Ecrit dans FILENAME la valeur de la tension avec timestamp en secondes
      dataFile = SD.open(FILENAME, FILE_WRITE);
      if(dataFile){
        String dataString = "";
        dataString += (gps.date.isValid() ? String(gps.date.day())+"/"+String(gps.date.month())+"/"+String(gps.date.year()) + ",": "null,");

        if(gps.time.isValid()){
          int heure = gps.time.hour()+2;
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

        // Besoin d'assigner en deux fois si on veut mixer les types
        String latlng = ",";
        latlng += String(gps.location.lat(),6) + "," + String(gps.location.lng(),6);
        dataString += (gps.location.isValid() ? latlng : "null");

        DISPLAY_PRINTLN(dataString);

        dataFile.println(dataString);
        dataFile.close();

        DISPLAY_PRINTLN(String(FILENAME) + " wrotten");
      }
      else
        DISPLAY_PRINTLN("Error opening "+String(FILENAME));
      break;
    case 3: // Lit le contenu de FILENAME
      dataFile = SD.open(FILENAME, FILE_READ);
      if(dataFile){
        DISPLAY_PRINTLN(String(FILENAME)+" will be read");

        while(dataFile.available())
          Serial.write(dataFile.read());
        dataFile.close();

        DISPLAY_PRINTLN(String(FILENAME)+" read");
      }
      else
        DISPLAY_PRINTLN("Can't open "+String(FILENAME));
      break;
    case 4: // Supprime FILENAME si le fichier existe
      if(SD.exists(FILENAME)){
        DISPLAY_PRINTLN(String(FILENAME)+" will be removed");
        if(SD.remove(FILENAME))
          DISPLAY_PRINTLN(String(FILENAME)+" removed");
        else
          DISPLAY_PRINTLN("Impossible to remove " + String(FILENAME));
      }
      else{
        DISPLAY_PRINTLN(String(FILENAME)+" does not exists");
      }
      break;
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

void display(mode m){
    if(m == menu){
      lcd.setCursor(0, 0);
      String date = String(gps.date.day())+"/"+String(gps.date.month())+"/"+String(gps.date.year());
      lcd.print(date);
      lcd.setCursor(0, 1);
      int heure  = gps.time.hour()+2;
      int minute = gps.time.minute();
      if(heure < 10)      lcd.print("0");
      lcd.print(heure);
      lcd.print(":");
      if(minute < 10)     lcd.print("0");
      lcd.print(gps.time.minute());
    }
    else if(m == timepassed){
      lcd.setCursor(0, 0);
      lcd.print(millis() / 1000);
      lcd.print("s");
    }
    else if(m == batterie){
      lcd.setCursor(0, 0);
      lcd.print("V: ");
      lcd.print(getTension());
    }
    else if(m == coordonnees){
      lcd.setCursor(0, 0);
      if(gps.location.isValid()){
        lcd.print(gps.location.lat(),6);
        lcd.setCursor(0, 1);
        lcd.print(gps.location.lng(),6);
      }
      else
        lcd.print("Process.");
    }
    else if(m == altitude){
      lcd.setCursor(0, 0);
      lcd.print("Altitude");
      lcd.setCursor(0, 1);
      lcd.print(gps.altitude.meters());
      lcd.print("m");
    }
    else if(m == hdop){
      lcd.setCursor(0, 0);
      lcd.print("HDOP:");
      lcd.setCursor(0, 1);
      lcd.print(gps.hdop.value());
    }
    else if(m == nbsat){
      lcd.setCursor(0, 0);
      lcd.print("NB SAT:");
      lcd.setCursor(0, 1);
      lcd.print(gps.satellites.value());
    }
    else{
      lcd.setCursor(0, 0);
      lcd.print("Error");
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
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
    Serial.print(F("INVALID"));

  Serial.println();
}

#endif
