#pragma once

void initWiFiScanner();
void startWiFiScan();

void initDeauthMode();
void sendDeauthPacket();

void scanForAPs();
void selectTargetAP(int targetId);
void startDeauth();
bool isDeauthRunning();       // returns whether attack is active
void handleDeauthLoop();      // sends one packet if it's time