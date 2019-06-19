#include "include/global.h"

TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX, GPS_TX);

void initGPS(){
  ss.begin(GPS_BAUD);
}

void runGPS(){
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
  }
  else
    Serial.print(F("INVALID"));

  Serial.println();
}

#endif
