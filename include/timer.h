#pragma once
#include <esp32-hal-timer.h>

// === Timer State ===
extern volatile bool joystickFlag;

// === Functions ===
void initTimer(int ms);

bool getFlag();

void setFlag(bool f);