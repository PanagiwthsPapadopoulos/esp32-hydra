#include <Arduino.h>
#include "joystick.h"

#define JOY_X_PIN 25
#define JOY_Y_PIN 26
#define JOY_BTN_PIN 27

JoystickDirection currentDirection = JoystickDirection::NONE;

void initJoystick() {
  pinMode(JOY_BTN_PIN, INPUT_PULLUP);  // Pull-up for button
}

void setDirection(JoystickDirection dir) {
  currentDirection = dir;
}

JoystickDirection getDirection() {
  return currentDirection;
}

void printJoystick() {
  int xValue = analogRead(JOY_X_PIN);
  int yValue = analogRead(JOY_Y_PIN);
  bool isPressed = digitalRead(JOY_BTN_PIN) == LOW;  // Active LOW button

  Serial.print("X: ");
  Serial.println(xValue);
  Serial.print(" | Y: ");
  Serial.println(yValue);
  Serial.print(" | Pressed: ");
  Serial.println(isPressed ? "YES" : "NO");
}

void getJoystickDirection() {
  int xValue = analogRead(JOY_X_PIN);
  int yValue = analogRead(JOY_Y_PIN);
  bool isPressed = digitalRead(JOY_BTN_PIN) == LOW;  // Active LOW button

  if(isPressed) setDirection(JoystickDirection::PRESS);
  else 
  if(xValue > 4000) setDirection(JoystickDirection::UP);
  else if (xValue < 300) setDirection(JoystickDirection::DOWN);
  else if (yValue < 300) setDirection(JoystickDirection::LEFT);
  else setDirection(JoystickDirection::NONE);
  // Serial.printf("Xvalue = %d\n", xValue);
  // Serial.println((int)getDirection());

}
