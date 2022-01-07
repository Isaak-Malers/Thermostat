#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Print.h>

#define LCD_ADDR 0x27
#define TEMP_ADDR 0x48

LiquidCrystal_I2C lcd(LCD_ADDR,16,2); 



void Heat(){
  lcd.setCursor(0, 0);
  lcd.print("Heating");
}

void Idle(){
  lcd.setCursor(0, 0);
  lcd.print("Idle   ");
}

void Cool(){
  lcd.setCursor(0, 0);
  lcd.print("Cooling");
}


void setup() {
  lcd.init();
  lcd.setBacklight(30);
  lcd.setDelay(0, 0);
  lcd.clear();
}

void loop() {
  Heat();
  delay(1500);
  Idle();
  delay(1500);
  Cool();
  delay(1500);
}

