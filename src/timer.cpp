#include <esp32-hal-timer.h>
#include <Arduino.h>

#include "timer.h"

#include "joystick.h"


hw_timer_t* timer = nullptr;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile bool joystickFlag = false;

void IRAM_ATTR onJoystickTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  joystickFlag = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void initTimer(int ms) {
  timer = timerBegin(0, 80, true);  // 80 prescaler -> 1μs per tick (80MHz/80 = 1MHz)
  timerAttachInterrupt(timer, &onJoystickTimer, true);
  timerAlarmWrite(timer, 100 * ms, true); // 100ms = 100,000 ticks
  timerAlarmEnable(timer);
}

bool getFlag() {
  return joystickFlag;
}

void setFlag(bool f) {
  portENTER_CRITICAL(&timerMux);
  joystickFlag = f;
  portEXIT_CRITICAL(&timerMux);
}