
#include "joystick.h"
#include "menu.h"
#include "timer.h"
#include "screen.h"





void setup() {
  Serial.begin(115200);

  initTimer(200);
  initScreen();
  initMenu();
}

void loop() {
  if (getFlag()) {
    getJoystickDirection();
    setFlag(false);

    updateMenu();

    if (getDirection() == JoystickDirection::PRESS) {
      handleMenuPress();
    }
  }
  // printJoystick();
}