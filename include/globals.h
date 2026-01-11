#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <M5Cardputer.h>

// Use decltype to get exact type
extern decltype(M5Cardputer) &Cardputer;
extern decltype(M5Cardputer.Keyboard) *Keyboard;

// Menu stuff
extern const int menuItemsCount;
extern const char *menuItems[];
extern int selectedItem;
extern String selection;
extern bool isInSubMenu;

#endif