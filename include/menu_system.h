#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "M5Cardputer.h"
#include <Arduino.h>

struct ListItem {
  String mainText;
  String subText1;
  String subText2;
  uint16_t mainColor;
  uint16_t subColor1;
  uint16_t subColor2;
  void *data; // Optional pointer to additional data
};

// state machine for list navigation
enum ListState { LIST_SCANNING, LIST_BROWSING, LIST_SELECTED };

class ScrollableList {
private:
  ListItem *items;
  int itemCount;
  int currentIndex;
  ListState state;
  String selectedMainText;
  bool needsRedraw;

  String scanningMessage;
  String selectedTitle;
  String noItemsMessage;
  String exitInstructions;

public:
  ScrollableList(String scanMsg = "Scanning...",
                 String selTitle = "Selected Item",
                 String noItems = "No items found",
                 String exitInstr = "[r]escan  [e]xit");

  ~ScrollableList();

  void reset();
  void setScanning();
  void setBrowsing(ListItem *newItems, int count);
  void setSelected(int index);
  ListState getState() { return state; }

  bool handleInput(); // returns true if should exit
  void next();
  void previous();

  // rendering
  void draw();
  void drawScanning();
  void drawBrowsing();
  void drawSelected();

  // getters
  int getCurrentIndex() { return currentIndex; }
  String getSelectedText() { return selectedMainText; }
  ListItem *getCurrentItem();
};

void drawPaginationDots(int total, int current, int y);

#endif