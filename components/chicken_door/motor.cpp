#include "motor.h"
#include <Arduino.h>

MotorDriver::MotorDriver(int forwardPin, int backwardPin, int thresholdPin, int thresholdValue)
{
    this->forwardPin = forwardPin;
    this->backwardPin = backwardPin;
    this->thresholdPin = thresholdPin;
    this->thresholdValue = thresholdValue;

    this->lastThresholdMeasurement = millis();
    this->lastThreshold=0;
    this->lastStateChange = millis();

    pinMode(forwardPin, OUTPUT);
    pinMode(backwardPin, OUTPUT);
    pinMode(thresholdPin, INPUT);
}

void MotorDriver::loop(){
    long millisSinceLastStateChange = millis() - this->lastStateChange;

    this->applyState(this->state); 

    if (!this->overrideJamDetection && this->shouldStall()){
      if ( this->debug)Serial.println("STALL");
      this->backward();
    }

    long millisSinceLastStall = millis() - this->lastStall;
    
    if (this->stalled && millisSinceLastStall > 5000){
      this->stallRetries = 0;
    }
}

void MotorDriver::applyState(MOTOR_STATE state){
    if (state == FORWARD){
        digitalWrite(this->forwardPin, HIGH);
        digitalWrite(this->backwardPin, LOW);
    } else if (state == BACKWARD){
        digitalWrite(this->forwardPin, LOW);
        digitalWrite(this->backwardPin, HIGH);  
    } else {
        digitalWrite(this->forwardPin, LOW);
        digitalWrite(this->backwardPin, LOW);
    }
}

void MotorDriver::reverse(){
  if (this->state==MOTOR_STATE::FORWARD){
    this->backward();
  }else if (this->state==MOTOR_STATE::BACKWARD){
    this->forward();
  }
}

void MotorDriver::forward()
{
      this->setState(MOTOR_STATE::FORWARD);
}

void MotorDriver::backward()
{
 
  this->setState(MOTOR_STATE::BACKWARD);
}

void MotorDriver::stop()
{
    this->setState(MOTOR_STATE::STOPPED);
}

void MotorDriver::setState(MOTOR_STATE state){
  if (state==this->state) return;
  this->thresholdIndex=0;
  for (int i=0; i<THRESHOLD_COUNT;i++) this->thresholds[i] = 0;
  this->state = state;
  this->lastStateChange = millis();
}

MOTOR_STATE MotorDriver::getState()
{
    return this->state;
}

bool MotorDriver::shouldStall(){
     long diff = millis() - this->lastStateChange;

     if (this->state != MOTOR_STATE::FORWARD) return false;
  
    int threshold = analogRead(this->thresholdPin);
    ESP_LOGD(TAG, "threshold", threshold);


     if (this->debug) Serial.println("Thresh:"+String(threshold));
     if (this->state==MOTOR_STATE::STOPPED) return false;
     if (diff < 1000) return false;

    this->insertThreshold(threshold);
    int avg = this->thresholdAvg();
    if (avg - threshold > this->thresholdValue ){
      if (this->debug) Serial.println("should stall because avg("+String(avg)+")- thresh("+String(threshold)+") > thr("+String(this->thresholdValue)+")");
      return true;
    }
    return false;
}

void MotorDriver::insertThreshold(int threshold)
{
    long diff = millis() - this->lastThresholdMeasurement;
    if (diff < THRESHOLD_DELAY) return;
    this->lastThresholdMeasurement = millis();
    this->thresholdIndex++;
    this->thresholds[this->thresholdIndex] = threshold;
    if (this->thresholdIndex >= THRESHOLD_COUNT){
        this->thresholdIndex = 0;
    }
}

int MotorDriver::thresholdAvg()
{
  if (thresholdIndex==0) return 0;
    long sum = 0;
    for (int i = 0; i < THRESHOLD_COUNT; i++){
        sum += this->thresholds[i];
    }
    return sum / this->thresholdIndex;
}

