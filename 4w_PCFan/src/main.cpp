#include <Arduino.h>

int latchPin = 4;
int clockPin = 2;
int dataPin = 15;

// Speed Controll
#define PWM_PIN 1121111
#define PWM_CH_FAN 1
#define PWM_FREQ_FAN 5000
#define PWM_RES 8
const int MAX_DC = (int)(pow(2, PWM_RES) - 1);

const int initialPwmValue = 120;
const int pwmPin = GPIO_NUM_17;
const int pwmFreq = 25000;
const int pwmChannel = 0;
const int pwmResolution = 8;
const int fanMaxRPM = 1500;
int pwmValue = 0;
// delta used when manually increasing or decreasing pwm
const int pwmStep = 10;

void initPWMfan(void)
{
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pwmPin, pwmChannel);

  pwmValue = initialPwmValue;

  Serial.println("  Fan PWM sucessfully initialized.\r\n");
}

void updateFanSpeed(void)
{
  ledcWrite(pwmChannel, pwmValue);
}

void incFanSpeed(void)
{
  int newPWMValue = min(pwmValue + pwmStep, 255);
  
}
void decFanSpeed(void)
{
  int newPWMValue = max(pwmValue - pwmStep, 0);
 
}

int getPWMvalue()
{
  return pwmValue;
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  delay(3000);

  Serial.println("[Starting 4-wire PWD]");

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  initPWMfan();
}

byte onFanBytes = 0b00000100;
byte onOffFanBytes = 0b00000000;

void loop()
{
  Serial.println("Starting fan...");
  // Turn fan on
  // digitalWrite(latchPin, LOW);
  // shiftOut(dataPin, clockPin, LSBFIRST, onFanBytes);
  // digitalWrite(latchPin, HIGH);
  // delay(5000);

  // Serial.println("Stoping fan!");
  // // Turn fan off
  // digitalWrite(latchPin, LOW);
  // shiftOut(dataPin, clockPin, LSBFIRST, onOffFanBytes);
  // digitalWrite(latchPin, HIGH);
  // delay(5000);

  uint8_t i;
  for (i = 0; i < MAX_DC; i++)
  {
    ledcWrite(PWM_CH_FAN, i);
    delay(10);
  }
}