#include <LiquidCrystal.h>
#include "HC_SR04.h"
#include "CustomSymbols.h"
//-------------Keyboard-----------
#define SELECT 1
#define LEFT 2
#define UP 3
#define DOWN 4
#define RIGHT 5
#define RESET 6
#define NO_BUTTONS 0
#define KEYBOARD_PORT A0
//--------------------------------
#define TRIG_PORT 12
#define ECHO_PORT 11

LiquidCrystal lcd(8, 9, 4, 5, 6, 7 );

// Connect first sensor
HC_SR04 HC(TRIG_PORT, ECHO_PORT);

bool onceMode = false;
bool canPrint = true;
unsigned long timer = 0;
unsigned long animationFrames[4] = 
{
  0, 2000, 4000, 6000
};

void setup()
{
  // Connect COM
  Serial.begin(9600);
  // Connect display
  lcd.begin(16,2);
  // Create icons
  lcd.createChar(1, symbols.frontPanel);
  lcd.createChar(2, symbols.backPanel);
}

void loop()
{
  byte key = 0;

  key = buttonChecker(KEYBOARD_PORT);

  switch (key)
  {
  case UP:
    HC.ChangeMode(1);
    delay(250);
    break;
  case DOWN:
    onceMode = !onceMode;
    delay(250);
    break;
  case SELECT:
    timer = millis();
    if (!onceMode)
    {
      HC.CalcDistance();

      viewMode(HC.GetMode());

      viewDistance();
    }
    else
    {
      HC.CalcDistance();
      delay(250);
    }
    break;
  default:
    break;
  }
  //----------------------------------------------------
  //------------------Display output--------------------
  //----------------------------------------------------
  // Constant measurement mode
  if ((!onceMode) && (key != SELECT))
  {
    // Instruction
    if ((millis() - timer >= animationFrames[0]) && (millis() - timer < animationFrames[1]))
    {
      lcd.print("For measure hold ");
      lcd.setCursor(0, 1);
      lcd.print("SELECT");
    }
    if ((millis() - timer >= animationFrames[1]) && (millis() - timer < animationFrames[2]))
    {
      lcd.println("DOWN");
      lcd.setCursor(0, 1);
      lcd.print("For once measure");
    }
    if ((millis() - timer >= animationFrames[2]) && (millis() - timer < animationFrames[3]))
    {
      viewMode(HC.GetMode());

      lcd.print("UP");
      lcd.setCursor(0, 1);
      lcd.print("To change mode");
    }
    if (millis() - timer >= animationFrames[3])
    {
      timer = millis();
    }
  }
  // Single measurement mode
  if (onceMode)
  {
    // Output distance to rhe display
    if ((millis() - timer >= animationFrames[0]) && (millis() - timer < animationFrames[1]))
    {
      viewMode(HC.GetMode());
      viewDistance();
    }
    if ((millis() - timer >= animationFrames[1]) && (millis() - timer < animationFrames[2]))
    {
      lcd.println("SELECT");
      lcd.setCursor(0, 1);
      lcd.print("For measure");
    }
    if ((millis() - timer >= animationFrames[2]) && (millis() - timer < animationFrames[3]))
    {
      lcd.println("DOWN");
      lcd.setCursor(0, 1);
      lcd.print("For exit");
    }

    if (millis() - timer >= animationFrames[3])
    {
      timer = millis();
    }
  }
  //---------------------------------------
  //--------------Port working-------------
  //---------------------------------------
  // Current measurement mode
  if (canPrint)
  {
    canPrint = false;
    Serial.print("Current mode: ");
    switch (HC.GetMode())
    {
    case 1:
      Serial.println("front");
      break;
    case 2:
      Serial.println("back");
      break;
    }
    if (onceMode)
    {
      Serial.println("Once mode enabled");
    }
    else
    {
      Serial.println("Once mode desabled");
    }
    // Last calculated distance
    Serial.print("Last distance: ");
    Serial.println(HC.GetDistance());
    // Changing mode from PC
    Serial.println("To change mode send \'c\'");
    // Changing single mode
    Serial.println("To switch once mod send \'o\'");
    // Измерение с ПК
    Serial.println("For measure send \'m\'");
  }
  if (Serial.available() > 0)
  {
    canPrint = true;
    char data = Serial.read();
    if (data == 'c')
    {
      HC.ChangeMode(1);
    }
    if (data == 'o')
    {
      onceMode = !onceMode;
    }
    if (data == 'm')
    {
      HC.CalcDistance();
    }
  }

  delay(50);
  // Output clearing
  lcd.clear();
  serialClear();
}

void serialClear()
{
  while (Serial.available())
  {
    Serial.read();
  }
}

// Displaying the current mode symbol on the screen
void viewMode(byte mode)
{
  switch (mode)
  {
  case 1:
    lcd.print(char(1));
    lcd.setCursor(3, 0);
    break;
  case 2:
    lcd.print(char(2));
    lcd.setCursor(3, 0);
    break;
  default:
    break;
  }
}

void viewDistance()
{
  lcd.print("Distance to ");
  lcd.setCursor(0, 1);
  lcd.print("object: ");
  lcd.print(HC.GetDistance());
  lcd.print("cm.");
}
//----------------------------------------------------
//-----------------Keyboard working-------------------
//----------------------------------------------------
void connectKeyboard(byte portNumber)
{
  pinMode(portNumber, INPUT);
  digitalWrite(portNumber, LOW);
}

byte buttonChecker(int portNumber)
{
  connectKeyboard(portNumber);

  int code = analogRead(portNumber);
  byte button = NO_BUTTONS;

  if (code >= 766 && code <= 866) button = SELECT;
  if (code >= 169 && code <= 269) button = UP;
  if (code >= 356 && code <= 456) button = DOWN;
  if (code >= 0 && code <= 95) button = RIGHT;

  return button;
}
