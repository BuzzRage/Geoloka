#include <Arduino.h>
#include "include/global.h"

float tension = 0;
int btnEn = 0;

void setup() {
  Serial.begin(SERIAL_SPEED);
  pinMode(VBAT_PIN, INPUT);
  pinMode(btn[0],   INPUT);
  pinMode(btn[1],   INPUT);
  pinMode(btn[2],   INPUT);

  tension = getTension();

  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.print("V: ");
  lcd.print(tension);
  Serial.print("Tension: "+String(tension));

  lcd.setCursor(0, 1);
  btnEn = getBtn();
  lcd.print("Btn: ");
  lcd.print(btnEn);
  Serial.print("Bouton: "+String(btnEn));

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
  lcd.setCursor(0, 1);

  btnEn = getBtn();

  lcd.print("Btn: ");
  Serial.println("Bouton: "+String(btnEn));
  lcd.print(btnEn);

  String dataString = "At " + String(millis() / 1000) + "s - Tension: "+String(getTension()) + "\n";
  Serial.println(dataString);

  switch(btnEn){
    case 1:
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
      }
      else
        Serial.println("Error opening datalog.txt");
      while(getBtn()==1);
      break;
    case 2:
      lcd.clear();
      lcd.print(millis() / 1000);
      while(getBtn()==2);
      break;
    case 3:
      dataFile = SD.open("datalog.txt", FILE_READ);
      if(dataFile){
        while (dataFile.available())
          Serial.write(dataFile.read());
        dataFile.close();
      }
      else
        Serial.println("error opening datalog.txt");
      break;
    case 4:
      SD.remove("datalog.txt");
      break;
  }
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
