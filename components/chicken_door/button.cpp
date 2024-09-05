#include "button.h"

void fallbackCallback(){
    Serial.println("No callback set");
}

Button::Button(const int pin)
{
    this->pin = pin;
}

void Button::onDown(const fptr& callback)
{
    if (this->debug) Serial.println("Setting down callback");
    this->downCallback = callback;
}

void Button::onUp(const fptr& callback)
{
    if (this->debug) Serial.println("Setting up callback");
    this->upCallback = callback;
}

void Button::loop(){
    int input = digitalRead(this->pin);
    if (this->buttonPressed && input==LOW){
        if (this->upCallback){
            if (this->debug) Serial.println("button released");
            this->upCallback.value_or(fallbackCallback)();
        }
    }else if(!this->buttonPressed&&input==HIGH){
     if (this->downCallback){
            if (this->debug) Serial.println("button pressed");
            this->downCallback.value_or(fallbackCallback)();
        }
    }
    this->buttonPressed = input;
}

bool Button::isPressed()
{
    return this->buttonPressed;
}
