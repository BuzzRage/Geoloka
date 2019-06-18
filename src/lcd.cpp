#include "include/global.h"

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

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
