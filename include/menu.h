#pragma once
#include <TFT_eSPI.h>

enum class MenuState {
    MAIN_MENU,
    WIFI_SCANNER,
    WIFI_DEAUTH,
    WIFI_DEAUTH_ATTACK,
    BT_DEVICE_SELECT,
    SETTINGS_MENU,
    HEART_ASCII,
    // Add more states here
  };
  

void initMenu();

MenuState getMenuState();
void setMenuState(MenuState newMenuState);

void drawMenu();
void updateMenu();
void handleMenuPress();
const char* getSelectedItem();

void populateSecondaryMenu(char* items[], int count);