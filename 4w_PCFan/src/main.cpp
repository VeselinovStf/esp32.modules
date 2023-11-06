#include <Arduino.h>

int latchPin = 4;
int clockPin = 2;
int dataPin = 15;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(3000);

  Serial.println("[Starting 4-wire PWD]");

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

byte onFanBytes = 0b00000100;
byte onOffFanBytes = 0b00000000;

void loop()
{
    Serial.println("Starting fan...");
    // Turn fan on
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, onFanBytes);
    digitalWrite(latchPin, HIGH);
    delay(5000);

    Serial.println("Stoping fan!");
    // Turn fan off
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, onOffFanBytes);
    digitalWrite(latchPin, HIGH);
    delay(5000);
}