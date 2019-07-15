#include "SO2002A.h"
#include "LiquidCrystal.h"

SO2002A oled(33, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22);

/*
 * SO2002A    Arduino
 * VSS  1
 * VDD  2
 * Vo   3
 * RS   4 33 
 * R/W  5 31
 * E    6 30
 * DB0  7 29
 * DB1  8 28
 * DB2  9 27
 * DB3 10 26
 * DB4 11 25
 * DB5 12 24
 * DB6 13 23
 * DB7 14 22
 */

char *string = "Hello World!!";

void setup() {
  oled.begin(20, 2);
  oled.selectCharacterRom(SO2002A_CHARACTER_ROM_A);
  oled.setCursor(0, 0);
  oled.print(string);
}

void loop() {
  for (int i = 0; i < (20 - strlen(string)); i++) {
    oled.scrollDisplayRight();
    delay(200);
  }
  for (int i = 0; i < (20 - strlen(string)); i++) {
    oled.scrollDisplayLeft();
    delay(200);
  }
  
  for (int i = 0; i < 255; i++) {
    oled.setContrastControl(i);
    delay(10);
  }
}

