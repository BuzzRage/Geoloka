#ifndef LCD_H
#define LCD_H

#define LCD_RS                4
#define LCD_EN                5
#define LCD_D4                6
#define LCD_D5                7
#define LCD_D6                8
#define LCD_D7                9

#define LCD_COLS							8
#define LCD_ROWS							2

#define NB_MODES	6
enum mode { menu, wmode, batterie, coordonnees, altitude, hdop, nbsat};

void display(mode m);

#endif
