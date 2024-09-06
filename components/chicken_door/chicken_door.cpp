#include "esphome/core/log.h"
#include "Arduino.h"
#include "chicken_door.h"

namespace esphome {
namespace chicken_door {

static const char *TAG = "cover.chicken_door";

ChickenDoor::ChickenDoor(): motor(18, 19, 32, 1000){
  }

void ChickenDoor::setup() {
  pinMode(5, 0x09);
}

void ChickenDoor::set_threshold(int threshold) {
  this->threshold = threshold;
  this->motor.setThreshold(threshold);
}

void ChickenDoor::loop() {
  this->motor.loop();
  ESP_LOGD(TAG, "Received thresh: %f", this->motor.thresholdAvg());

}

void ChickenDoor::dump_config() {
  ESP_LOGCONFIG(TAG, "Empty cover");
}

cover::CoverTraits ChickenDoor::get_traits() {
  auto traits = cover::CoverTraits();
  traits.set_is_assumed_state(false);
  traits.set_supports_position(false);
  traits.set_supports_tilt(false);

  return traits;
}

void ChickenDoor::control(const cover::CoverCall &call) {
  if (call.get_position().has_value()) {
    float pos = *call.get_position();
    ESP_LOGD(TAG, "Received position: %f", pos);


    if (pos>0.5){
       this->motor.forward();
      this->position = 1.0;
       ESP_LOGD(TAG, "pos = 1 < 0.5");
    }else{
       this->motor.backward();
      this->position = 0.0;
      ESP_LOGD(TAG, "pos = 0.0");
    }

     this->publish_state();
  }
}

}  // namespace empty_cover
}  // namespace esphome