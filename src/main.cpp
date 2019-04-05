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

  lcd.begin(8, 2);
  lcd.print("V: ");
  Serial.print("Tension: "+String(tension));
  lcd.print(tension);

  lcd.setCursor(0, 1);
  btnEn = getBtn();
  lcd.print("Btn: ");
  Serial.print("Bouton: "+String(btnEn));
  lcd.print(btnEn);

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
  //lcd.print(millis() / 1000);
  delay(10);
  btnEn = getBtn();
  lcd.print("Btn: ");
  Serial.print("Bouton: "+String(btnEn));
  lcd.print(btnEn);

  String dataString = "Tension: "+String(getTension()) + "\n";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }


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
