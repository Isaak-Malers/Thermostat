#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Temperature_LM75_Derived.h>


#define LCD_ADDR 0x27
#define TEMP_ADDR 0x48

LiquidCrystal_I2C lcd(LCD_ADDR,16,2); 
Generic_LM75 temperature(TEMP_ADDR);

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

void UpdateTemp(){
  float tempC = temperature.readTemperatureC();
  float tempF = (tempC*1.8) + 32;
  lcd.setCursor(9, 0);
  lcd.print(tempF);
  lcd.print("F");
}


void setup() {
  lcd.init();
  lcd.setBacklight(30);
  lcd.setDelay(0, 0);
  lcd.clear();
}

void loop() {
  UpdateTemp();
  Heat();
  delay(250);
  Idle();
  delay(250);
  Cool();
  delay(250);
}

