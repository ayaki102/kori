#include "menu_system.h"
#include "globals.h"
#include "ui.h"
#include "utils.h"

ScrollableList::ScrollableList(String scanMsg, String selTitle, String noItems,
                               String exitInstr)
    : items(nullptr), itemCount(0), currentIndex(0), state(LIST_SCANNING),
      needsRedraw(true), scanningMessage(scanMsg), selectedTitle(selTitle),
      noItemsMessage(noItems), exitInstructions(exitInstr) {}

ScrollableList::~ScrollableList() {
  if (items != nullptr) {
    delete[] items;
  }
}

void ScrollableList::reset() {
  state = LIST_SCANNING;
  currentIndex = 0;
  selectedMainText = "";
  needsRedraw = true;
  if (items != nullptr) {
    delete[] items;
    items = nullptr;
  }
  itemCount = 0;
}

void ScrollableList::setScanning() {
  state = LIST_SCANNING;
  needsRedraw = true;
}

void ScrollableList::setBrowsing(ListItem *newItems, int count) {
  if (items != nullptr) {
    delete[] items;
  }
  items = newItems;
  itemCount = count;
  currentIndex = 0;
  state = LIST_BROWSING;
  needsRedraw = true;
}

void ScrollableList::setSelected(int index) {
  if (index >= 0 && index < itemCount) {
    currentIndex = index;
    selectedMainText = items[index].mainText;
    state = LIST_SELECTED;
    needsRedraw = true;
  }
}

bool ScrollableList::handleInput() {
  bool shouldExit = false;

  switch (state) {
  case LIST_SELECTED:
    if (isKeyPressed('r') || isKeyPressed('R')) {
      reset();
      return false;
    }
    if (isKeyPressed('e') || isKeyPressed('E')) {
      return true; // Exit to main menu
    }
    break;

  case LIST_BROWSING:
    if (isKeyPressed('r') || isKeyPressed('R')) {
      reset();
      return false;
    }
    if (isKeyPressed('/')) {
      next();
    }
    if (isKeyPressed(',')) {
      previous();
    }
    if (isKeyPressed(KEY_ENTER) && itemCount > 0) {
      setSelected(currentIndex);
    }
    break;

  case LIST_SCANNING:
    // No input handling during scanning
    break;
  }

  return shouldExit;
}

void ScrollableList::next() {
  if (itemCount > 0) {
    currentIndex++;
    if (currentIndex >= itemCount) {
      currentIndex = 0;
    }
    needsRedraw = true;
  }
}

void ScrollableList::previous() {
  if (itemCount > 0) {
    currentIndex--;
    if (currentIndex < 0) {
      currentIndex = itemCount - 1;
    }
    needsRedraw = true;
  }
}

ListItem *ScrollableList::getCurrentItem() {
  if (currentIndex >= 0 && currentIndex < itemCount) {
    return &items[currentIndex];
  }
  return nullptr;
}

void ScrollableList::draw() {
  switch (state) {
  case LIST_SCANNING:
    drawScanning();
    break;
  case LIST_BROWSING:
    drawBrowsing();
    break;
  case LIST_SELECTED:
    drawSelected();
    break;
  }
}

void ScrollableList::drawScanning() {
  if (!needsRedraw)
    return;

  int w = Cardputer.Display.width();
  int h = Cardputer.Display.height();

  clearScrn();
  Cardputer.Display.setTextColor(TFT_WHITE);
  Cardputer.Display.setTextDatum(MC_DATUM);
  Cardputer.Display.drawString(scanningMessage, w / 2, h / 2);

  needsRedraw = false;
}

void ScrollableList::drawBrowsing() {
  if (!needsRedraw)
    return;

  int w = Cardputer.Display.width();
  int h = Cardputer.Display.height();

  clearScrn();
  drawBattery();

  if (itemCount == 0) {
    Cardputer.Display.setTextDatum(MC_DATUM);
    Cardputer.Display.setTextColor(TFT_WHITE);
    Cardputer.Display.drawString(noItemsMessage, w / 2, h / 2);
  } else {
    ListItem *item = &items[currentIndex];

    Cardputer.Display.setTextDatum(MC_DATUM);

    // Draw main text
    Cardputer.Display.setTextColor(item->mainColor);
    if (M5Cardputer.Display.textWidth(item->mainText) > w - 2) {
      int len = item->mainText.length();
      int mid = len / 2;
      String top = item->mainText.substring(0, mid);
      String bottom = item->mainText.substring(mid);
      Cardputer.Display.drawString(top, w / 2, h / 2 - 40);
      Cardputer.Display.drawString(bottom, w / 2, h / 2 - 20);
    } else {
      Cardputer.Display.drawString(item->mainText, w / 2, h / 2 - 20);
    }

    // Draw sub text 1
    if (item->subText1.length() > 0) {
      Cardputer.Display.setTextColor(item->subColor1);
      Cardputer.Display.drawString(item->subText1, w / 2, h / 2);
    }

    // Draw sub text 2
    if (item->subText2.length() > 0) {
      Cardputer.Display.setTextColor(item->subColor2);
      Cardputer.Display.drawString(item->subText2, w / 2, h / 2 + 20);
    }

    // Draw pagination dots
    drawPaginationDots(itemCount, currentIndex, h - 20);
  }

  needsRedraw = false;
}

void ScrollableList::drawSelected() {
  if (!needsRedraw)
    return;

  int w = Cardputer.Display.width();
  int h = Cardputer.Display.height();

  clearScrn();
  drawBattery();

  Cardputer.Display.setTextColor(TFT_WHITE);
  Cardputer.Display.setTextDatum(MC_DATUM);
  Cardputer.Display.drawString(selectedTitle, w / 2, 20);

  Cardputer.Display.setTextColor(TFT_PURPLE);
  if (M5Cardputer.Display.textWidth(selectedMainText) > w - 2) {
    int len = selectedMainText.length();
    int mid = len / 2;
    String top = selectedMainText.substring(0, mid);
    String bottom = selectedMainText.substring(mid);
    Cardputer.Display.drawString(top, w / 2, h / 2 - 10);
    Cardputer.Display.drawString(bottom, w / 2, h / 2 + 10);
  } else {
    Cardputer.Display.drawString(selectedMainText, w / 2, h / 2);
  }

  Cardputer.Display.setTextColor(TFT_DARKGREY);
  Cardputer.Display.drawString(exitInstructions, w / 2, h - 20);

  needsRedraw = false;
}

void drawPaginationDots(int total, int current, int y) {
  int w = Cardputer.Display.width();
  int dotSpacing = 15;
  int startX = (w / 2) - ((total - 1) * dotSpacing / 2);

  if (total > 10) {
    startX = (w / 2) - (9 * dotSpacing / 2);
  }

  int dotsToShow = total > 10 ? 10 : total;

  for (int i = 0; i < dotsToShow; i++) {
    int dotX = startX + (i * dotSpacing);
    uint16_t color;
    if (i == current) {
      color = TFT_PURPLE;
      Cardputer.Display.fillCircle(dotX, y, 4, color);
    } else {
      color = TFT_DARKGREY;
      Cardputer.Display.fillCircle(dotX, y, 2, color);
    }
  }
}