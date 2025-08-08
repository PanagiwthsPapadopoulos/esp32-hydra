#include "menu.h"

#include "joystick.h"
#include "screen.h"
#include <wifi_tools.h>

const char* menuItems[] = {
  "Wi-Fi Scanner",
  "Bluetooth Jammer",
  "Deauth Attack",
  "Sniffer Mode",
  "Settings",
  "Anda <3"
};
const int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);
int selectedItem = 0;
bool mainMenu = true;
TFT_eSPI* tft = getTFT();

void initMenu() {
  drawMenu();
}

void drawMenu() {
  tft->fillScreen(TFT_BLACK);
  tft->setTextSize(2);  // Double size text (default is 1)
  for (int i = 0; i < menuItemCount; i++) {
    if (i == selectedItem) {
      tft->setTextColor(TFT_BLACK, TFT_GREEN);
    } else {
      tft->setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft->drawString(menuItems[i], 20, 30 + i * 30);
  }
}

void updateMenu() {
  int previousItem = selectedItem;
  int direction = 0;
//   Serial.println((int)getDirection());

  switch (getDirection()) {
    case JoystickDirection::UP: 
        direction = -1; 
        Serial.println("Joystick up");
        break;
    case JoystickDirection::DOWN: 
        direction = 1; 
        Serial.println("Joystick down");
        break;
    case JoystickDirection::LEFT:
        if(!mainMenu) drawMenu();
        mainMenu = true;
    default: 
        direction = 0; 
        break;
  }
//   Serial.printf("UpdateMenu func started! Direction = %d\n", direction);
  if(mainMenu){
    selectedItem = (selectedItem + direction + menuItemCount) % menuItemCount;
    if(previousItem != selectedItem) drawMenu();
  } else selectedItem = previousItem;
}

void displayHeartAscii() {
    const char* heartAscii[] = {
      "   ***     ***   ",
      "  *****   *****  ",
      "  *************  ",
      "   ***********   ",
      "    *********    ",
      "     *******     ",
      "      *****      ",
      "       ***       ",
      "        *        "
    };
  
    tft->fillScreen(TFT_BLACK);
    tft->setTextColor(TFT_RED, TFT_BLACK);
    tft->setTextSize(2);
  
    int x = 20;
    int y = 30;
    for (int i = 0; i < sizeof(heartAscii) / sizeof(heartAscii[0]); i++) {
      tft->drawString(heartAscii[i], x, y + i * 10);
    }
  
    tft->setTextColor(TFT_PINK, TFT_BLACK);
    tft->drawString("I <3 U Anda", 50, y + 120);
    // delay(2000);
  }


void handleMenuPress() {
  mainMenu = false;
  switch (selectedItem) {
    case 0:
      initWiFiScanner();
      startWiFiScan();
      break;
    case 5:
      displayHeartAscii();
      break;
    default:
      tft->fillScreen(TFT_BLACK);
      tft->setTextColor(TFT_GREEN);
      tft->drawString("Selected:", 20, 30);
      tft->drawString(menuItems[selectedItem], 20, 60);
      // delay(1000);
      break;
  }

  // drawMenu();
}

const char* getSelectedItem() {
  return menuItems[selectedItem];
}



  