#include "globals.h"

decltype(M5Cardputer) &Cardputer = M5Cardputer;
decltype(M5Cardputer.Keyboard) *Keyboard = &Cardputer.Keyboard;

const char *menuItems[] = {"WiFi Scan", "Deautoryzacja", "BLE skan", "Opcje"};

const int lenMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);
int selectedItem = 0;
int currentMenuIndex = 0;
bool isInSubMenu = false;
float currentLineX = 0;
float currentLineWidth = 0;
float targetLineX = 0;
float targetLineWidth = 0;
float prevLineX = 0;
float prevLineWidth = 0;
