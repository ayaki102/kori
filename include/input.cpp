#include "input.h"
#include "globals.h"
#include "ui.h"

void handleMenuInput() {
  if (Cardputer.Keyboard.isKeyPressed(',')) {
    selectedItem = (selectedItem - 1 + menuItemsCount) % menuItemsCount;
    drawMenu();
  } else if (Cardputer.Keyboard.isKeyPressed('/')) {
    selectedItem = (selectedItem + 1) % menuItemsCount;
    drawMenu();
  } else if (Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
    isInSubMenu = true;
    drawSubmenu();
  }
}

void handleSubmenuInput() {
  if (Cardputer.Keyboard.isKeyPressed(KEY_BACKSPACE) ||
      Cardputer.Keyboard.isKeyPressed('`')) {
    isInSubMenu = false;
    drawMenu();
  }
  // Add submenu-specific controls here
}

void handleInput() {
  if (isInSubMenu) {
    handleSubmenuInput();
  } else {
    handleMenuInput();
  }
}