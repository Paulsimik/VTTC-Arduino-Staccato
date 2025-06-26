//#############################
// VTTC Staccato
// Version 1.2
// By Paul Simik
//#############################

//  1.0
//  - Program release
//
//  1.1
//  - Added option pulse delay for Standart mode - pot2
//  - Added Falling ARSG Mode
//  - Added Bidirectional ARSG Mode
//  - Small fixes
//  1.2
//  - Optimalizing

#include <Arduino.h>

#define INPUT_PIN 3
#define OUTPUT_PIN 2
#define OUTPUT_LED 13
#define POT_1 A0
#define POT_2 A1
#define POT_3 A2
#define BTN_1 5
#define BTN_2 6
#define BTN_3 7
#define BTN_4 8

enum modeType
{
  MODE_STANDART,
  MODE_BURST,
  MODE_RISING,
  MODE_FALLING,
  MODE_BIDIRECTIONAL
};

enum modeType currMode = MODE_STANDART;

int pot1 = 0;
int pot2 = 0;
int pot3 = 0;

bool btn_1 = false;
bool btn_2 = false;
bool btn_3 = false;
bool btn_4 = false;

int i = 9700;
int x = 0;
int y = 0;
int z = 0;

bool pulse = false;
bool rising = true;

void Output();
void FallingMode();
void FallingControl();
void RisingMode();
void RisingControl();
void BidirectionalMode();
void BidirectionalControl();
void BurstMode();
void BurstControl();
void StandartMode();
void StandartControl();
void SetOutput(bool output);
void SetupPins();
int SelectMode();

void setup() 
{
  SetupPins();
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), Output, RISING);
}

void loop() 
{
  switch (SelectMode())
  {
    case MODE_BIDIRECTIONAL:  currMode = MODE_BIDIRECTIONAL;    BidirectionalControl(); break;
    case MODE_FALLING:        currMode = MODE_FALLING;          FallingControl();       break;
    case MODE_RISING:         currMode = MODE_RISING;           RisingControl();        break;
    case MODE_BURST:          currMode = MODE_BURST;            BurstControl();         break;
    case MODE_STANDART:       currMode = MODE_STANDART;         StandartControl();      break;
  }
}

void Output()
{
  switch (currMode)
  {
    case MODE_BIDIRECTIONAL:  BidirectionalMode();  break;
    case MODE_FALLING:        FallingMode();        break;
    case MODE_RISING:         RisingMode();         break;
    case MODE_BURST:          BurstMode();          break;
    case MODE_STANDART:       StandartMode();       break;
  }
}

void FallingMode()
{
  x++;
  if(x >= pot1)
  {
    y += pot2;
    if(y < 9700)
    {
      delayMicroseconds(y);
      SetOutput(1);
      delayMicroseconds(9700 - y);
      SetOutput(0);
    }

    x = 0;
  }

  if(y > pot3)
  {
    y = 0;
  }
}

void FallingControl()
{
  pot1 = map(analogRead(POT_1), 0, 1023, 2, 8); // Frequency
  pot2 = map(analogRead(POT_2), 0, 1023, 500, 50); // Rising steps
  pot3 = map(analogRead(POT_3), 0, 1023, 9000, 20000); // Delay next start
}

void RisingMode()
{
  x++;
  if(x >= pot1)
  {
    i -= pot2;
    if(i > 0)
    {
      delayMicroseconds(i);
      SetOutput(1);
      delayMicroseconds(9700 - i);
      SetOutput(0);
    }

    x = 0;
  }

  if(i < (pot3 * -1))
  {
    i = 9700;
  }
}

void RisingControl()
{
  pot1 = map(analogRead(POT_1), 0, 1023, 2, 8); // Frequency
  pot2 = map(analogRead(POT_2), 0, 1023, 500, 50); // Falling steps
  pot3 = map(analogRead(POT_3), 0, 1023, 200, 20000); // Delay next start
}

void BidirectionalMode()
{
  x++;
  if(x >= pot1)
  {
    if(rising)
    {
      i -= pot2;
      if(i > 0)
      {
        delayMicroseconds(i);
        SetOutput(1);
        delayMicroseconds(9700 - i);
        SetOutput(0);
      }

      x = 0;

      if(i < 500)
      {
        i = 9700;
        rising = false;
      }
    }
    else
    {
      y += pot2;
      if(y < 9700)
      {
        delayMicroseconds(y);
        SetOutput(1);
        delayMicroseconds(9700 - y);
        SetOutput(0);
      }

      x = 0;

      if(y > pot3)
      {
        y = 0;
        rising = true;
      }
    }
  }
}

void BidirectionalControl()
{
  pot1 = map(analogRead(POT_1), 0, 1023, 2, 8); // Frequency
  pot2 = map(analogRead(POT_2), 0, 1023, 500, 50); // Rising steps
  pot3 = map(analogRead(POT_3), 0, 1023, 9000, 30000); // Delay next start
}

void BurstMode()
{
  x++;
  if(x >= pot3)
  {
    z++;
    if(z >= pot2)
    {
      SetOutput(1);
      delayMicroseconds(9500);
      SetOutput(0);
      y++;
      z = 0;
    }

    if(y >= pot1)
    {
      x = 0;
      y = 0;
    }
  }
}

void BurstControl()
{
  pot1 = map(analogRead(POT_1), 0, 1023, 20, 2); // Pulse count
  pot2 = map(analogRead(POT_2), 0, 1023, 2, 10); // Pulse skip
  pot3 = map(analogRead(POT_3), 0, 1023, 10, 100);  // Pulse delay
}

void StandartMode()
{
  x++;
  if(x >= pot1)
  {
    delayMicroseconds(pot2);
    SetOutput(1);
    delayMicroseconds(9700 - pot2);
    SetOutput(0);
    x = 0;
  }
}

void StandartControl()
{
  pot1 = map(analogRead(POT_1), 0, 1023, 2, 100); // Standart Mode pulse skip
  pot2 = map(analogRead(POT_2), 0, 1023, 0, 9700); // Start pulse delay
}

int SelectMode()
{
  if(digitalRead(BTN_1))
    return MODE_BIDIRECTIONAL;
  else if (digitalRead(BTN_2))
    return MODE_RISING;
  else if (digitalRead(BTN_3))
    return MODE_FALLING;
  else if (digitalRead(BTN_4))
    return MODE_BURST;
  else
    return MODE_STANDART;
}

void SetOutput(bool output)
{
  output ? PORTD |= (1 << PIND2) : PORTD &= ~(1 << PIND2);
  output ? PORTB |= (1 << PINB5) : PORTB &= ~(1 << PINB5);
}

void SetupPins()
{
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(OUTPUT_LED, OUTPUT);
  pinMode(INPUT_PIN, INPUT);
  pinMode(POT_1, INPUT);
  pinMode(POT_2, INPUT);
  pinMode(POT_3, INPUT);
  pinMode(BTN_1, INPUT);
  pinMode(BTN_2, INPUT);
  pinMode(BTN_3, INPUT);
  pinMode(BTN_4, INPUT);
}
