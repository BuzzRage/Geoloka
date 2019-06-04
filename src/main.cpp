#include <Arduino.h>
#include "include/global.h"

float tension = 0;

int btnEn = 0;
int btn[3] = {BP0, BP1, BPEN};

enum mode current_mode = menu;
File dataFile;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
SoftwareSerial ss(GPS_RX, GPS_TX);
TinyGPSPlus gps;


void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(VBAT_PIN, INPUT);
  pinMode(btn[0],   INPUT_PULLUP);
  pinMode(btn[1],   INPUT_PULLUP);
  pinMode(btn[2],   INPUT_PULLUP);

  lcd.begin(LCD_COLS, LCD_ROWS);

  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");

  initGPS();
}

void loop() {
  btnEn = getBtn();
  switch(btnEn){
    case 1:
      if(current_mode < NB_MODES){
        current_mode = (mode)current_mode + 1;
        lcd.clear();
      }
      else
        current_mode = 0;
        lcd.clear();
      break;
    case 2: // Ecrit dans FILENAME la valeur de la tension avec timestamp en secondes
      dataFile = SD.open(FILENAME, FILE_WRITE);
      if (dataFile){
        //String dataString = "At " + String(millis() / 1000) + "s - Tension: "+String(getTension()) + "\n";
        String dataString = "";
        dataString += (gps.date.isValid() ? String(gps.date.month())+"/"+String(gps.date.day())+"/"+String(gps.date.year()) + " - ": "DATE INVALID - ");

        if (gps.time.isValid()){
          if (gps.time.hour() < 10) dataString += "0";
          dataString += gps.time.hour() + ":";
          if (gps.time.minute() < 10) dataString += "0";
          dataString += gps.time.minute() + ":";
          if (gps.time.second() < 10) dataString += "0";
          dataString += gps.time.second() + ".";
          if (gps.time.centisecond() < 10) dataString += "0";
          dataString += gps.time.centisecond() + " - ";
        }
        else{
          dataString += "TIME INVALID - ";
        }
        dataString += ("(" + gps.location.isValid() ? String(gps.location.lat()) + "," + String(gps.location.lng()) + ") - "          : "LOCATION INVALID - ");

        DISPLAY_PRINTLN(dataString);
        dataFile.println(dataString);
        dataFile.close();
        Serial.println(String(FILENAME) + " wrotten");
      }
      else
        Serial.println("Error opening "+String(FILENAME));
      break;
    case 3: // Lit le contenu de FILENAME
      dataFile = SD.open(FILENAME, FILE_READ);
      if(dataFile){
        Serial.println(String(FILENAME)+" will be read");
        while (dataFile.available())
          Serial.write(dataFile.read());
        dataFile.close();
        Serial.println(String(FILENAME)+" read");
      }
      else
        Serial.println("Can't open "+String(FILENAME));
      break;
    case 4: // Supprime FILENAME si le fichier existe
      if(SD.exists(FILENAME)){
        Serial.println(String(FILENAME)+" will be removed");
        SD.remove(FILENAME);
        Serial.println(String(FILENAME)+" removed");
      }
      else{
        Serial.println(String(FILENAME)+" does not exists");
      }
      break;
  }

  display(current_mode);
  testGPS();

  while(getBtn()!=0);
  //delay(10);
}

int getBtn(){
  bool btnVal[3] = {false,false,false};
  btnVal[0] = digitalRead(btn[0]);
  btnVal[1] = digitalRead(btn[1]);
  btnVal[2] = digitalRead(btn[2]);

  if     (!btnVal[2] &&  btnVal[1] &&  btnVal[0])
    return 0;
  else if( btnVal[2] && !btnVal[1] && !btnVal[0])
    return 1;
  else if( btnVal[2] && !btnVal[1] &&  btnVal[0])
    return 2;
  else if( btnVal[2] &&  btnVal[1] && !btnVal[0])
    return 3;
  else if( btnVal[2] &&  btnVal[1] &&  btnVal[0])
    return 4;
  else
    return -1;
}

float getTension(){
  return (float(analogRead(VBAT_PIN))/1023.0)*6.0;
}

// TODO: affichage coordonnee + fixe btn bounce + lecture/ecriture carte SD + niveau de batterie

void display(mode m){
  if(m == menu){
    lcd.setCursor(0, 0);
    lcd.print("V: ");
    lcd.print(getTension());
    lcd.setCursor(0, 1);
    lcd.print("Btn: ");
    lcd.print(getBtn());
    DISPLAY_PRINTLN("Bouton: "+String(getBtn()));
  }
  else if(m == timepassed){
    lcd.setCursor(0, 0);
    lcd.print(millis() / 1000);
    lcd.print("s");
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
}

void initGPS(){
  ss.begin(GPS_BAUD);
}

void testGPS(){
  while(ss.available() > 0)
    if(gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10){
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

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
    Serial.print(gps.time.hour());
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
