#include "ui.h"
#include "globals.h"

void drawMenu() {
  Cardputer.Display.fillScreen(TFT_BLACK);
  delay(100);

  int h = Cardputer.Display.height();
  int w = Cardputer.Display.width();

  // === BATTERY - Top Right ===
  int batLvl = Cardputer.Power.getBatteryLevel();
  uint16_t batColor = batLvl > 50   ? TFT_GREEN
                      : batLvl > 20 ? TFT_ORANGE
                                    : TFT_RED;

  Cardputer.Display.setTextDatum(top_right);
  Cardputer.Display.setTextSize(1);
  Cardputer.Display.setTextColor(batColor);
  Cardputer.Display.drawString(String(batLvl) + "%", w - 2, 2);

  Cardputer.Display.setTextDatum(middle_center);
  Cardputer.Display.setTextSize(2);
  Cardputer.Display.setTextColor(TFT_WHITE);
  Cardputer.Display.drawString(menuItems[selectedItem], w / 2, h / 2);

  // Glowing underline effect (double line)
  int textWidth = Cardputer.Display.textWidth(menuItems[selectedItem]);
  int lineY = h / 2 + 18;
  Cardputer.Display.drawLine(w / 2 - textWidth / 2 - 4, lineY,
                             w / 2 + textWidth / 2 + 4, lineY, TFT_VIOLET);
  Cardputer.Display.drawLine(w / 2 - textWidth / 2 - 4, lineY + 1,
                             w / 2 + textWidth / 2 + 4, lineY + 1,
                             0x780F); // Darker violet

  // Accent corner brackets around text
  int bracketY = h / 2 - 20;
  int bracketX = w / 2 - textWidth / 2 - 12;
  int bracketSize = 8;

  // Top-left bracket
  Cardputer.Display.drawLine(bracketX, bracketY, bracketX + bracketSize,
                             bracketY, TFT_VIOLET);
  Cardputer.Display.drawLine(bracketX, bracketY, bracketX,
                             bracketY + bracketSize, TFT_VIOLET);

  // Top-right bracket
  Cardputer.Display.drawLine(w - bracketX, bracketY, w - bracketX - bracketSize,
                             bracketY, TFT_VIOLET);
  Cardputer.Display.drawLine(w - bracketX, bracketY, w - bracketX,
                             bracketY + bracketSize, TFT_VIOLET);

  // Bottom-left bracket
  Cardputer.Display.drawLine(bracketX, h / 2 + 28, bracketX + bracketSize,
                             h / 2 + 28, TFT_VIOLET);
  Cardputer.Display.drawLine(bracketX, h / 2 + 28, bracketX,
                             h / 2 + 28 - bracketSize, TFT_VIOLET);

  // Bottom-right bracket
  Cardputer.Display.drawLine(w - bracketX, h / 2 + 28,
                             w - bracketX - bracketSize, h / 2 + 28,
                             TFT_VIOLET);
  Cardputer.Display.drawLine(w - bracketX, h / 2 + 28, w - bracketX,
                             h / 2 + 28 - bracketSize, TFT_VIOLET);

  Cardputer.Display.setTextSize(1);
  Cardputer.Display.setTextColor(TFT_DARKGREY);
  Cardputer.Display.setTextDatum(bottom_left);
  Cardputer.Display.drawString("< PREV", 4, h - 2);

  Cardputer.Display.setTextDatum(bottom_right);
  Cardputer.Display.drawString("NEXT >", w - 4, h - 2);

  // === PAGINATION DOTS - Enhanced ===
  int dotY = h - 16;
  int totalDots = menuItemsCount;
  const int dotSpacing = 12;
  int startX = w / 2 - (totalDots * dotSpacing) / 2;

  for (int i = 0; i < totalDots; i++) {
    int dotX = startX + i * dotSpacing;

    if (i == selectedItem) {
      // Active dot - filled with glow
      Cardputer.Display.fillCircle(dotX, dotY, 3, TFT_VIOLET);
      Cardputer.Display.drawCircle(dotX, dotY, 4, 0x780F); // Outer glow
    } else {
      // Inactive dots
      Cardputer.Display.drawCircle(dotX, dotY, 2, TFT_DARKGREY);
    }
  }

  // === CORNER ACCENTS - Screen edges ===
  int cornerSize = 6;
  uint16_t cornerColor = 0x39C7; // Dim cyan

  // Top corners
  Cardputer.Display.drawLine(0, 0, cornerSize, 0, cornerColor);
  Cardputer.Display.drawLine(0, 0, 0, cornerSize, cornerColor);
  Cardputer.Display.drawLine(w - 1, 0, w - cornerSize - 1, 0, cornerColor);
  Cardputer.Display.drawLine(w - 1, 0, w - 1, cornerSize, cornerColor);

  // Bottom corners
  Cardputer.Display.drawLine(0, h - 1, cornerSize, h - 1, cornerColor);
  Cardputer.Display.drawLine(0, h - 1, 0, h - cornerSize - 1, cornerColor);
  Cardputer.Display.drawLine(w - 1, h - 1, w - cornerSize - 1, h - 1,
                             cornerColor);
  Cardputer.Display.drawLine(w - 1, h - 1, w - 1, h - cornerSize - 1,
                             cornerColor);
}

void drawSubmenu() {
  int h = Cardputer.Display.height();
  int w = Cardputer.Display.width();
  Cardputer.Display.fillScreen(TFT_BLACK);
  Cardputer.Display.setTextColor(TFT_BLUE);
  Cardputer.Display.drawString(menuItems[selectedItem], w / 2, h / 2);
}