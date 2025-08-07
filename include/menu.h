#pragma once
#include <TFT_eSPI.h>

void initMenu();
void drawMenu();
void updateMenu();
void handleMenuPress();
const char* getSelectedItem();