#include "globals.h"

const char *menuItems[] = {"WiFi Scan", "Deautoryzacja", "BLE skan", "Opcje"};
const int menuItemsCount = sizeof(menuItems) / sizeof(menuItems[0]);
int selectedItem = 0;
int currentMenuIndex = 0;
bool isInSubMenu = false;