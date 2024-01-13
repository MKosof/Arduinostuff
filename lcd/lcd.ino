#include <LiquidCrystal.h>

const int rs=5, en=4, d5=14, d6=12, d7=13, d4=2;
LiquidCrystal lcd(rs, en, d7, d6, d5, d4);


void setup() {
  // set up the LCD's number of columns and rows:
  delay(500);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("This been on for:");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  uint seconds=millis()/1000;
  uint minutes=seconds/60;
  uint hours=minutes/60;
  char time[9];
  snprintf(time,9,"%d:%d:%d",hours,minutes%60,seconds%60);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(time);
  delay(1000);
}


