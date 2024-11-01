#include <Arduino.h>

#define OUTPUT_PIN PA0
#define INPUT_PIN PB11
#define BPS_PIN PA7

int maxBps = 2;

int i = 0;

void Output_ISR(void);

void setup() 
{
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT);
  pinMode(BPS_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), Output_ISR, RISING);
}

void loop() 
{
  maxBps = map(analogRead(BPS_PIN), 0, 1023, 2, 100);
}

void Output_ISR()
{
  i++;
  if(i >= maxBps)
  {
    digitalWrite(OUTPUT_PIN, HIGH);
    delayMicroseconds(9700); //9700 period
    digitalWrite(OUTPUT_PIN, LOW);
    i = 0;
  }
}