#include "utils.h"
#include "globals.h"

bool isKeyPressed(char c) { return Cardputer.Keyboard.isKeyPressed(c); }
void clearScrn() { Cardputer.Display.fillScreen(TFT_BLACK); }