#include "BLEDevice.h"
#include "BLEScan.h"
#include "BLEUtils.h"
#include "M5Cardputer.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include "globals.h"
#include "menu_system.h"
#include "ui.h"
#include "utils.h"

void handleMenu();
void handleWiFiScan();
void handleBLEScan();
void handleOptions();

// WiFi scanning globals
ScrollableList *wifiList = nullptr;
bool wifiScanInProgress = false;

// BLE scanning globals (for future use)
ScrollableList *bleList = nullptr;
bool bleScanInProgress = false;

void setup() {
  auto cfg = M5.config();
  Serial.begin(115200);
  Cardputer.begin(cfg, true);
  int textsize = Cardputer.Display.height() / 60;
  if (textsize == 0)
    textsize = 1;
  Cardputer.Display.setTextSize(textsize);
  clearScrn();

  int w = Cardputer.Display.width();
  int16_t textWidth = Cardputer.Display.textWidth(menuItems[selectedItem]);
  currentLineX = (w / 2) - (textWidth / 2);
  currentLineWidth = textWidth;
  targetLineX = currentLineX;
  targetLineWidth = currentLineWidth;
  prevLineX = currentLineX;
  prevLineWidth = currentLineWidth;

  drawMenu();
}

void loop() {
  Cardputer.update();

  if (Cardputer.Keyboard.isChange()) {
    if (!isInSubMenu) {
      handleMenu();
    } else {
      // Handle submenu navigation based on selected item
      if (isKeyPressed(KEY_BACKSPACE)) {
        isInSubMenu = false;
        drawMenu();
      } else if (selectedItem == MenuItem::WIFI_SCAN) {
        handleWiFiScan();
      } else if (selectedItem == MenuItem::BLE_SCAN) {
        handleBLEScan();
      } else if (selectedItem == MenuItem::OPTIONS) {
        handleOptions();
      }
    }
  }

  if (abs(currentLineX - targetLineX) > 0.5 ||
      abs(currentLineWidth - targetLineWidth) > 0.5) {
    updateAnimation(0.3);
    drawLineOnly();
  }

  delay(16);
}

void handleMenu() {
  if (isKeyPressed('/')) {
    selectedItem++;
    if (selectedItem >= lenMenuItems) {
      selectedItem = 0;
    }
    updateLineTarget(menuItems[selectedItem]);
    drawMenu();
  }
  if (isKeyPressed(',')) {
    selectedItem--;
    if (selectedItem < 0) {
      selectedItem = lenMenuItems - 1;
    }
    updateLineTarget(menuItems[selectedItem]);
    drawMenu();
  }
  if (isKeyPressed(KEY_ENTER)) {
    isInSubMenu = true;

    if (selectedItem == MenuItem::WIFI_SCAN) {
      // Initialize WiFi list if needed
      if (wifiList == nullptr) {
        wifiList = new ScrollableList("Scanning WiFi...", "Selected Network",
                                      "No networks found", "[r]escan  [e]xit");
      }
      wifiList->reset();
      wifiScanInProgress = false;
      handleWiFiScan();
    } else if (selectedItem == MenuItem::BLE_SCAN) {
      // Initialize BLE list if needed
      if (bleList == nullptr) {
        bleList = new ScrollableList("Scanning BLE...", "Selected Device",
                                     "No devices found", "[r]escan  [e]xit");
      }
      bleList->reset();
      bleScanInProgress = false;
      handleBLEScan();
    } else if (selectedItem == MenuItem::OPTIONS) {
      handleOptions();
    }
  }
}

void handleWiFiScan() {
  if (wifiList == nullptr)
    return;

  // Handle scanning state
  if (wifiList->getState() == LIST_SCANNING && !wifiScanInProgress) {
    wifiList->drawScanning();

    // Perform the actual WiFi scan
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int n = WiFi.scanNetworks(false, true);

    // Create list items from scan results
    ListItem *items = new ListItem[n];
    for (int i = 0; i < n; i++) {
      items[i].mainText = WiFi.SSID(i);
      items[i].mainColor = TFT_WHITE;

      // Signal strength
      items[i].subText1 = String(WiFi.RSSI(i)) + " dBm";
      items[i].subColor1 = TFT_CYAN;

      // Security type
      wifi_auth_mode_t encryption = WiFi.encryptionType(i);
      if (encryption == WIFI_AUTH_OPEN) {
        items[i].subText2 = "Open";
      } else if (encryption == WIFI_AUTH_WEP) {
        items[i].subText2 = "WEP";
      } else if (encryption == WIFI_AUTH_WPA_PSK) {
        items[i].subText2 = "WPA";
      } else if (encryption == WIFI_AUTH_WPA2_PSK) {
        items[i].subText2 = "WPA2";
      } else if (encryption == WIFI_AUTH_WPA_WPA2_PSK) {
        items[i].subText2 = "WPA/WPA2";
      } else {
        items[i].subText2 = "WPA3";
      }
      items[i].subColor2 = TFT_MAGENTA;
    }

    wifiList->setBrowsing(items, n);
    wifiScanInProgress = true;
    return;
  }

  // Handle input and check for exit
  if (wifiList->handleInput()) {
    isInSubMenu = false;
    drawMenu();
    return;
  }

  // Draw current state
  wifiList->draw();
}

void handleBLEScan() {
  if (bleList == nullptr)
    return;

  // Handle scanning state
  if (bleList->getState() == LIST_SCANNING && !bleScanInProgress) {
    bleList->drawScanning();

    // TODO: Perform the actual BLE scan
    // For now, just create dummy data as an example
    ListItem *items = new ListItem[3];

    items[0].mainText = "BLE Device 1";
    items[0].mainColor = TFT_WHITE;
    items[0].subText1 = "-45 dBm";
    items[0].subColor1 = TFT_CYAN;
    items[0].subText2 = "AA:BB:CC:DD:EE:FF";
    items[0].subColor2 = TFT_MAGENTA;

    items[1].mainText = "BLE Device 2";
    items[1].mainColor = TFT_WHITE;
    items[1].subText1 = "-67 dBm";
    items[1].subColor1 = TFT_CYAN;
    items[1].subText2 = "11:22:33:44:55:66";
    items[1].subColor2 = TFT_MAGENTA;

    items[2].mainText = "BLE Device 3";
    items[2].mainColor = TFT_WHITE;
    items[2].subText1 = "-82 dBm";
    items[2].subColor1 = TFT_CYAN;
    items[2].subText2 = "AA:11:BB:22:CC:33";
    items[2].subColor2 = TFT_MAGENTA;

    bleList->setBrowsing(items, 3);
    bleScanInProgress = true;
    return;
  }

  // Handle input and check for exit
  if (bleList->handleInput()) {
    isInSubMenu = false;
    drawMenu();
    return;
  }

  // Draw current state
  bleList->draw();
}

void handleOptions() {
  // TODO: Implement options menu
}