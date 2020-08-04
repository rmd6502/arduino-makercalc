#include "Adafruit_Keypad.h"
#include <Wire.h>
#include <LiquidTWI2.h>
#include "calculator.h"

// Connect via i2c, address 0x20 (A0-A2 not jumpered)
LiquidTWI2 lcd(0x20);

const byte ROWS = 5; // rows
const byte COLS = 5; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'0', '1','2','3','<'},
  {'4','5','6','7', '^'},
  {'8','9','A','B','>'},
  {'C','.','E','F','='},
  {'+','-','*','/','S'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3, A6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {D3, D9, D10, D6, D2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Calculator calculator;
uint32_t lastKey = 0;
bool backlight = true;
const uint32_t SAVER_TIME = 20000;

void setup() {
  Serial.begin(115200);
  lcd.setMCPType(LTI_TYPE_MCP23017); 
  // set up the LCD's number of rows and columns:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Calculator v0.01");
  lcd.setBacklight(BLUE);
  lastKey = millis();
  customKeypad.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    Serial.print((char)e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) {
      if (!backlight) {
        backlight = true;
        lcd.setBacklight(BLUE);
      }
      lastKey = millis();
      Serial.println(" pressed");
      calculator.input(e.bit.KEY);
      Serial.print("result "); Serial.println(calculator.result);
      lcd.setCursor(0,1);
      lcd.print(calculator.result);
      for (int i=calculator.result_pos; i < 16; ++i) lcd.print(' ');
    }
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
  }

  delay(10);
  if (backlight && millis() - lastKey > SAVER_TIME) {
    lcd.setBacklight(LOW);
    backlight = false;
  }
}
