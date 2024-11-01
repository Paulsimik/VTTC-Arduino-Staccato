//#############################
// VTTC Staccato
// Version 1.0.1
// By Paul Simik
//#############################

// 1.0.0
// - Program release
//
// 1.0.1
// - Added option pulse delay for Standart mode - pot2
// - Added Falling ARSG Mode
// - Added Bidirectional ARSG Mode
// - Small fixes

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

void Output(void);
void FallingARSGMode(void);
void RisingARSGMode(void);
void ARSGMode(void);
void BurstMode(void);
void StandartMode(void);
void SetOutput(bool output);

void setup() 
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

  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), Output, RISING);
}

void loop() 
{
  btn_1 = digitalRead(BTN_1);
  btn_2 = digitalRead(BTN_2);
  btn_3 = digitalRead(BTN_3);
  btn_4 = digitalRead(BTN_4);

  if(btn_1) // Bidirectional ARSG
  {
    pot1 = map(analogRead(POT_1), 0, 1023, 2, 8); // Frequency
    pot2 = map(analogRead(POT_2), 0, 1023, 500, 50); // Rising steps
    pot3 = map(analogRead(POT_3), 0, 1023, 9000, 30000); // Delay next start
    return;
  }

  if(btn_2) // Rising ARSG
  {
    pot1 = map(analogRead(POT_1), 0, 1023, 2, 8); // Frequency
    pot2 = map(analogRead(POT_2), 0, 1023, 500, 50); // Falling steps
    pot3 = map(analogRead(POT_3), 0, 1023, 200, 20000); // Delay next start
    return;
  }

  if(btn_3) // Falling ARSG
  {
    pot1 = map(analogRead(POT_1), 0, 1023, 2, 8); // Frequency
    pot2 = map(analogRead(POT_2), 0, 1023, 500, 50); // Rising steps
    pot3 = map(analogRead(POT_3), 0, 1023, 9000, 20000); // Delay next start
    return;
  }

  if(btn_4) // Burst
  {
    pot1 = map(analogRead(POT_1), 0, 1023, 20, 2); // Pulse count
    pot2 = map(analogRead(POT_2), 0, 1023, 2, 10); // Pulse skip
    pot3 = map(analogRead(POT_3), 0, 1023, 10, 100);  // Pulse delay
    return;
  }

  // Standart
  pot1 = map(analogRead(POT_1), 0, 1023, 2, 100); // Standart Mode pulse skip
  pot2 = map(analogRead(POT_2), 0, 1023, 0, 9700); // Start pulse delay
}

void Output()
{
  if(btn_1)
  {
    ARSGMode();
    return;
  }
  
  if(btn_2)
  {
    RisingARSGMode();
    return;
  }

  if(btn_3)
  {
    FallingARSGMode();
    return;
  }

  if(btn_4)
  {
    BurstMode();
    return;
  }

  StandartMode();
}

void FallingARSGMode()
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

void RisingARSGMode()
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

void ARSGMode()
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

void SetOutput(bool output)
{
  digitalWrite(OUTPUT_PIN, output);
  digitalWrite(OUTPUT_LED, output); 
}