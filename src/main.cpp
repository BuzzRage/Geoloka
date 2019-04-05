#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal.h>

int aIn = 14;
float tension = 0;
const int rs = 4, en = 5, d4 = 6, d5 = 7, d6 = 8, d7 = 9;
int btn[3] = {16,15,17};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int btnEn = 0;

const int cs = 10, mosi = 11, miso = 12, sck = 13;

int getBtn();
float getTension();

void setup() {
  Serial.begin(9600);
  pinMode(aIn, INPUT);
  pinMode(btn[0], INPUT);
  pinMode(btn[1], INPUT);
  pinMode(btn[2], INPUT);

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
  if (!SD.begin(cs)) {
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
  return (float(analogRead(aIn))/1023.0)*6.0;
}
