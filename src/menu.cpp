#include "menu.h"

#include "joystick.h"
#include "screen.h"
#include <wifi_tools.h>

const char* menuItems[] = {
  "Wi-Fi Scanner",
  "Deauth Attack",
  "Bluetooth Jammer",
  "Sniffer Mode",
  "Settings",
  "Anda <3"
};

char** menuItemsSecondary = nullptr;

const int menuItemCount = sizeof(menuItems) / sizeof(menuItems[0]);
int menuItemCountSecondary = 0;

int selectedItem = 0;
TFT_eSPI* tft = getTFT();

MenuState currentMenu = MenuState::MAIN_MENU;


void initMenu() {
  drawMenu();
}

MenuState getMenuState() {
  return currentMenu;
}

void setMenuState(MenuState newMenuState) {
  currentMenu = newMenuState;
  menuItemCountSecondary = 0;
  selectedItem = 0;
  drawMenu();
}

void drawMenu() {
  tft->fillScreen(TFT_BLACK);
  tft->setTextSize(2);  // Double size text (default is 1)

  if (currentMenu == MenuState::MAIN_MENU) {
    for (int i = 0; i < menuItemCount; i++) {
      if (i == selectedItem) {
        tft->setTextColor(TFT_BLACK, TFT_GREEN);
      } else {
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
      }
      tft->drawString(menuItems[i], 20, 30 + i * 30);
    }
  } else {
    for (int i = 0; i < menuItemCountSecondary; i++) {
      if (i == selectedItem) {
        tft->setTextColor(TFT_BLACK, TFT_GREEN);
      } else {
        tft->setTextColor(TFT_WHITE, TFT_BLACK);
      }
      tft->drawString(menuItemsSecondary[i], 20, 30 + i * 30);  // ✅ fixed this line
    }
  }
}

void updateMenu() {
  int previousItem = selectedItem;
  int direction = 0;

  // Serial.printf("Menu = %d\n", (int)currentMenu);

  // Default UP/DOWN navigation for all menus
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
        if(currentMenu != MenuState::MAIN_MENU){
          Serial.println("Joystick left");
          setMenuState(MenuState::MAIN_MENU);
          
        }
        break;
    default: 
        direction = 0; 
        break;
  }

  // Update selected item variable
  int itemCount = (currentMenu == MenuState::MAIN_MENU) ? menuItemCount : menuItemCountSecondary;
  if(itemCount>0)selectedItem = (selectedItem + direction + itemCount) % itemCount;
  // If another item is selected, redraw menu
  if(previousItem != selectedItem) drawMenu();

  
  
//   Serial.printf("UpdateMenu func started! Direction = %d\n", direction);

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

  if(currentMenu == MenuState::WIFI_DEAUTH) {
    setMenuState(MenuState::WIFI_DEAUTH_ATTACK);
    selectTargetAP(selectedItem);
    startDeauth();
  } else if(currentMenu == MenuState::MAIN_MENU) {
    switch (selectedItem) {
      case 0:
        setMenuState(MenuState::WIFI_SCANNER);
        initWiFiScanner();
        startWiFiScan();
        break;
      case 1:
        setMenuState(MenuState::WIFI_DEAUTH);
        scanForAPs();
        break;
      case 5:
        setMenuState(MenuState::HEART_ASCII);
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
  }

  // drawMenu();
}

const char* getSelectedItem() {
  return menuItems[selectedItem];
}


void populateSecondaryMenu(char* items[], int count) {
  menuItemsSecondary = items;
  menuItemCountSecondary = count;
  drawMenu();
}
  