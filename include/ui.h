#ifndef UI_H
#define UI_H

#include <Arduino.h>

void drawMenu();
void updateLineTarget(String text);
void updateAnimation(float smooth);
void drawLineOnly();
void clearScrn();
void drawBattery();

#endif
