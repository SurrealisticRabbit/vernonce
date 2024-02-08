#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

LiquidCrystal_I2C lcd(0x27,20,4); // set the LCD address to 0x27 and col and row to 20,4

void init_lcd(){
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Waiting for host");
  for (int c = 0; c < 5; c++){
    for (int i = 0; i < 4; i++){
      lcd.print(".");
      delay(150);
    }
    lcd.setCursor(0,0);
    lcd.print("Waiting for host    ");

    lcd.clear();
    lcd.home();
    lcd.print("Waiting for host");
  }
}

void setup()
{
  init_lcd();
  Serial.begin(9600);
}

void loop() 
{
  if (Serial.available()){ // if available read the serial data

    uint8_t data = Serial.read();

    if (data == 0xFE){

      while (Serial.available() == 0) {}

      uint8_t secondary_data = Serial.read();
      switch (secondary_data) {
        case 0x47: { // Set Cursor position
          while (Serial.available() < 2) {}

          uint8_t parameter_x = Serial.read();
          uint8_t parameter_y = Serial.read();

          lcd.setCursor(parameter_x, parameter_y);
          break;
        }
        case 0x48: { // Cursor Home
          lcd.home();
          break;
        }
        case 0x58: { // Clear LCD
          lcd.clear();
          break;
        }
        default: { // If no instruction print data
          lcd.print(data);
          lcd.print(secondary_data);
          break;
        }
      }
    }
    else { // Print data
      lcd.write(data);
    }
  }
}
