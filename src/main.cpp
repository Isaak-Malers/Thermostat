#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Temperature_LM75_Derived.h>


#define LCD_ADDR 0x27
#define TEMP_ADDR 0x48
#define HeatPin 4
#define CoolPin 5
#define FanPin 6

LiquidCrystal_I2C lcd(LCD_ADDR,16,2); 
Generic_LM75 temperature(TEMP_ADDR);

byte tempSamples[16];
byte tempIndex;
byte tempAverage;

byte highTemp = 82;
byte lowTemp = 68;
byte debounceDegrees = 3;

//int minOnTime = 300000; //5 minutes
int minOnTime = 1500;

bool coolMethod = 0; //0 for fan, 1 for AC
bool idle = 0;//begin in a non-idle state:  this will be more aggressive about getting back to the ideal temperature range.

void Heat(){
  idle = false;
  lcd.setCursor(0, 0);
  lcd.print("Heat");
  pinMode(HeatPin, OUTPUT);
}

void Cool(){
  idle = false;
  lcd.setCursor(0, 0);
  if(coolMethod){
    lcd.print("A/C ");
    pinMode(CoolPin, OUTPUT);
  }else{
    lcd.print("FAN ");
    pinMode(FanPin, OUTPUT);
  }
}

void Fan(){
  idle = false;
  lcd.setCursor(0, 0);
  lcd.print("Atk Fan");
  pinMode(FanPin, OUTPUT);
  idle = false;
}

void Idle(){
  idle = true;
  lcd.setCursor(0, 0);
  lcd.print("Idle");
  pinMode(HeatPin, INPUT);
  pinMode(CoolPin, INPUT);
}

void updateCoolMethod(){
  lcd.setCursor(8, 0);
  if(coolMethod){
    lcd.print("FAN [AC]");
  }else{
    lcd.print("[FAN] AC");
  }
}

void UpdateTemp(){
  //float tempC = temperature.readTemperatureC();
  //byte tempF = (tempC*1.8) + 32;
  byte tempF = map(analogRead(A0), 0, 1023, 60, 99);
  tempSamples[tempIndex] = tempF;
  tempIndex++;
  if(tempIndex == 16){
    tempIndex = 0;
  }
  double sum = 0;
  for(byte i=0; i<16; i++){
    sum = sum + tempSamples[i];
  };
  tempAverage = sum/16;
  lcd.setCursor(0, 1);
  lcd.print(lowTemp);
  lcd.print("F");

  lcd.setCursor(7, 1);
  lcd.print(tempAverage);
  lcd.print("F");

  lcd.setCursor(13, 1);
  lcd.print(highTemp);
  lcd.print("F");
}

int main()
{
  init();


  lcd.init();
  lcd.setBacklight(15);
  lcd.setDelay(0, 0);
  lcd.clear();
  lcd.print("Starting....");

  for(byte i = 0; i<20; i++){
    UpdateTemp();
    delay(150);
  }
  lcd.clear();

  for(;;){
    UpdateTemp();
    updateCoolMethod();

    //Addin some debouncing so the furnace can run for a reasonable amount of time.
    byte th = highTemp;
    byte tl = lowTemp;
    if(idle == false){
      th = th - debounceDegrees;
      tl = tl + debounceDegrees;
    }

    if(tempAverage <= tl){
      Heat();
      delay(minOnTime);
      continue;
    }else if(tempAverage >= th){
      Cool();
      delay(minOnTime);
      continue;
    }else{
      Idle();
      delay(minOnTime);
      continue;
    }
  }
}