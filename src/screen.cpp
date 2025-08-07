#include <TFT_eSPI.h>

TFT_eSPI screen = TFT_eSPI();

void initScreen() {
    screen.init();
    screen.setRotation(0);     // set to landscape mode
}

TFT_eSPI* getTFT() {
    return &screen;
}