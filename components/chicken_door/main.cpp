#include "Arduino.h"
#include "motor.h"
#include "button.h"
#include "config.h"

const int inputPin = 33;
const int endstop = 5;

int status = 0;

MotorDriver motor(18, 19, 32, 250);
Button button(33);

long last_millis;
long button_pressed = false;

void onButtonPress()
{
  Serial.println("Button pressed");
  status++;
  if (status > 2)
    status = 0;
  switch (status)
  {
  case 0:
    motor.backward();
    break;
  case 1:
    motor.forward();
    break;
  case 2:
    motor.stop();
    break;
  default:
    motor.stop();
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLDOWN);
  pinMode(endstop, INPUT_PULLDOWN);
  last_millis = millis();
  motor.overrideJamDetection = false;
  motor.debug = DEBUG_MOTOR;
  button.debug = DEBUG_BUTTON;

  button.onDown(onButtonPress);
  Serial.println("Setup complete");
}

void loop()
{
  motor.loop();
  button.loop();

  int state = digitalRead(endstop);

  long diff = millis() - last_millis;
  if (diff > 1000)
  {
    last_millis = millis();
    if (state == HIGH)
    {
      Serial.println("Endstop hit");
    }
    else
    {
      Serial.println("Endstop not hit");
    }
  }
}
