#ifndef GLOBALS_H
#define GLOBALS_H

#include <M5Cardputer.h>

extern decltype(M5Cardputer) &Cardputer;
extern decltype(M5Cardputer.Keyboard) *Keyboard;

enum MenuItem {
  WIFI_SCAN = 0,
  DEAUTH = 1,
  BLE_SCAN = 2,
  OPTIONS = 3,
  MENU_ITEM_COUNT
};

extern const char *menuItems[];
extern const int lenMenuItems;
extern int selectedItem;
extern int currentMenuIndex;
extern bool isInSubMenu;

extern float currentLineX;
extern float currentLineWidth;
extern float targetLineX;
extern float targetLineWidth;
extern float prevLineX;
extern float prevLineWidth;

extern int h;
extern int w;

#endif