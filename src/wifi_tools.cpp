#include <WiFi.h>
#include <WiFiType.h>
#include "wifi_tools.h"
#include "screen.h"  // for getTFT()

TFT_eSPI* tft_wifi = getTFT();

void initWiFiScanner() {
  WiFi.mode(WIFI_STA);    // Set to station mode
  WiFi.disconnect(true);  // Disconnect from any previous network
  delay(100);
}

void startWiFiScan() {
  tft_wifi->fillScreen(TFT_BLACK);
  tft_wifi->setTextColor(TFT_GREEN);
  tft_wifi->drawString("Scanning...", 20, 30);

  int n = WiFi.scanNetworks();
  if(WiFi.scanComplete() >= 0){
    if (n == 0) {
      tft_wifi->drawString("No networks found", 20, 60);
    } else {
      for (int i = 0; i < n && i < 10; ++i) { 
        String ssid = WiFi.SSID(i);
        int rssi = WiFi.RSSI(i);
        String label = ssid + " (" + rssi + "dBm)";
        tft_wifi->drawString(label, 20, 60 + i * 20);
      }
    }
    tft_wifi->drawString("<- to return", 20, 280);  // user instruction
    WiFi.scanDelete(); // Clear results
    WiFi.mode(WIFI_OFF);
  }
  // delay(3000);  // Delay so user sees the results
}