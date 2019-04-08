#include <Arduino.h>
#include "include/global.h"

float tension = 0;
int btnEn = 0;
int mode = 0;
File dataFile;

void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(VBAT_PIN, INPUT);
  pinMode(btn[0],   INPUT);
  pinMode(btn[1],   INPUT);
  pinMode(btn[2],   INPUT);

  lcd.begin(LCD_COLS, LCD_ROWS);

  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  btnEn = getBtn();
  display(mode);

  switch(btnEn){
    case 1:
      dataFile = SD.open(FILENAME, FILE_WRITE);
      if (dataFile) {
        String dataString = "At " + String(millis() / 1000) + "s - Tension: "+String(getTension()) + "\n";
        DISPLAY_PRINTLN(dataString);
        dataFile.println(dataString);
        dataFile.close();
        Serial.println(String(FILENAME) + " wrotten");
      }
      else
        Serial.println("Error opening "+String(FILENAME));
      break;
    case 2:
      if(mode != 1){
        mode = 1;
        lcd.clear();
      }
      else
        mode = 0;
        lcd.clear();
      break;
    case 3:
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
    case 4:
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

  while(getBtn()!=0);
  delay(10);
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

void display(int mode){
  if(mode == 0){
    lcd.setCursor(0, 0);
    lcd.print("V: ");
    lcd.print(getTension());
    lcd.setCursor(0, 1);
    lcd.print("Btn: ");
    lcd.print(getBtn());
    DISPLAY_PRINTLN("Bouton: "+String(getBtn()));
  }
  else if(mode == 1){
    lcd.setCursor(0, 0);
    lcd.print(millis() / 1000);
    lcd.print("s");
  }
}
