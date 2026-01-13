#include "ui.h"
#include "globals.h"
#include "utils.h"

esp_err_t res;

void updateLineTarget(String text) {
  int w = Cardputer.Display.width();
  int16_t textWidth = Cardputer.Display.textWidth(text);
  targetLineX = (w / 2) - (textWidth / 2);
  targetLineWidth = textWidth;
}

void updateAnimation(float smooth) {
  float smoothness = smooth;

  prevLineX = currentLineX;
  prevLineWidth = currentLineWidth;

  currentLineX += (targetLineX - currentLineX) * smoothness;
  currentLineWidth += (targetLineWidth - currentLineWidth) * smoothness;
}

void drawLineOnly() {
  int h = Cardputer.Display.height();
  int lineY = (h / 2) + 18;

  int oldX1 = (int)prevLineX;
  int oldX2 = (int)(prevLineX + prevLineWidth);
  int newX1 = (int)currentLineX;
  int newX2 = (int)(currentLineX + currentLineWidth);

  int eraseStart = min(oldX1, newX1);
  int eraseEnd = max(oldX2, newX2);

  for (int i = 0; i < 3; i++) {
    Cardputer.Display.drawLine(eraseStart, lineY + i, eraseEnd, lineY + i,
                               TFT_BLACK);
  }

  for (int i = 0; i < 3; i++) {
    Cardputer.Display.drawLine(newX1, lineY + i, newX2, lineY + i, TFT_PURPLE);
  }
}

void drawMenu() {
  int h = Cardputer.Display.height();
  int w = Cardputer.Display.width();

  clearScrn();
  drawBattery();

  Cardputer.Display.setTextColor(TFT_WHITE);
  Cardputer.Display.setTextDatum(MC_DATUM);
  Cardputer.Display.drawString(menuItems[selectedItem], w / 2, h / 2);

  int lineY = (h / 2) + 18;
  for (int i = 0; i < 3; i++) {
    Cardputer.Display.drawLine((int)currentLineX, lineY + i,
                               (int)(currentLineX + currentLineWidth),
                               lineY + i, TFT_PURPLE);
  }

  int dotY = h - 20;
  int dotSpacing = 15;
  int startX = (w / 2) - ((lenMenuItems - 1) * dotSpacing / 2);

  for (int i = 0; i < lenMenuItems; i++) {
    int dotX = startX + (i * dotSpacing);
    uint16_t color;
    if (i == selectedItem) {
      color = TFT_PURPLE;
      Cardputer.Display.fillCircle(dotX, dotY, 4, color);
    } else {
      color = TFT_DARKGREY;
      Cardputer.Display.fillCircle(dotX, dotY, 2, color);
    }
  }
}

void drawBattery() {
  int w = Cardputer.Display.width();
  Cardputer.Display.setTextSize(1);
  int batteryInt = Cardputer.Power.getBatteryLevel();
  String battery = String(batteryInt) + "%";
  if (batteryInt >= 80) {
    Cardputer.Display.setTextColor(TFT_WHITE);
  } else if (batteryInt >= 50) {
    Cardputer.Display.setTextColor(TFT_MAGENTA);
  } else {
    Cardputer.Display.setTextColor(TFT_RED);
  }

  Cardputer.Display.setTextDatum(TR_DATUM);
  Cardputer.Display.drawString(battery, w - 10, 10);
  int textsize = Cardputer.Display.height() / 60;
  if (textsize == 0)
    textsize = 1;
  Cardputer.Display.setTextSize(textsize);
}