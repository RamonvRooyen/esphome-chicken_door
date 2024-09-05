#include "esphome/core/log.h"
#include "Arduino.h"
#include "chicken_door.h"

namespace esphome {
namespace chicken_door {

static const char *TAG = "cover.chicken_door";

ChickenDoor::ChickenDoor(): motor(18, 19, 32, 250){
  }

void ChickenDoor::setup() {
  pinMode(5, 0x09);
}

void ChickenDoor::loop() {
  this->motor.loop();
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
    if (pos<0.5){
      pos+=1;
    }
    if ((int)pos==0){
      this->motor.forward();
    }else{
      this->motor.backward();
     }
     *call.set_position(pos);
     this->publish_state();
  }
}

}  // namespace empty_cover
}  // namespace esphome