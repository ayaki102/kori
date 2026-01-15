
#include "BLEDevice.h"
#include "BLEScan.h"
#include "BLEUtils.h"
#include "M5Cardputer.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include "globals.h"
#include "ui.h"
#include "utils.h"

void handleMenu();
void handleWiFiScan();
void handleOptions();
void resetWiFiScanState();

int currNetwork = 0;
bool scanned = false;
int n = 0;

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
      resetWiFiScanState();
      handleWiFiScan();
    } else if (selectedItem == MenuItem::OPTIONS) {
      handleOptions();
    }
  }
}

void handleWiFiScan() {
  // Use enum for clearer state management
  enum ScanState { SCANNING, BROWSING, SELECTED };
  static ScanState state = SCANNING;
  static String selectedSSID = "";
  static bool needsRedraw = true;

  // Check if we just entered (scanned will be false from reset)
  if (!scanned && state != SCANNING) {
    state = SCANNING;
    needsRedraw = true;
  }

  int w = Cardputer.Display.width();
  int h = Cardputer.Display.height();

  // STATE: SELECTED NETWORK
  if (state == SELECTED) {
    if (needsRedraw) {
      clearScrn();
      drawBattery();
      Cardputer.Display.setTextColor(TFT_CYAN);
      Cardputer.Display.setTextDatum(MC_DATUM);
      Cardputer.Display.drawString("Selected Network", w / 2, 20);

      Cardputer.Display.setTextColor(TFT_WHITE);
      if (M5Cardputer.Display.textWidth(selectedSSID) > w - 2) {
        int len = selectedSSID.length();
        int mid = len / 2;
        String top = selectedSSID.substring(0, mid);
        String bottom = selectedSSID.substring(mid);
        Cardputer.Display.drawString(top, w / 2, h / 2 - 10);
        Cardputer.Display.drawString(bottom, w / 2, h / 2 + 10);
      } else {
        Cardputer.Display.drawString(selectedSSID, w / 2, h / 2);
      }

      Cardputer.Display.setTextColor(TFT_DARKGREY);
      Cardputer.Display.drawString("[r]escan  [e]xit", w / 2, h - 20);
      needsRedraw = false;
    }

    if (isKeyPressed('r') || isKeyPressed('R') || isKeyPressed(KEY_ENTER)) {
      state = SCANNING;
      selectedSSID = "";
      scanned = false;
      currNetwork = 0;
      needsRedraw = true;
    }

    if (isKeyPressed('e') || isKeyPressed('E') || isKeyPressed(KEY_BACKSPACE)) {
      isInSubMenu = false;
      drawMenu();
    }
    return;
  }

  // STATE: SCANNING
  if (state == SCANNING && !scanned) {
    clearScrn();
    Cardputer.Display.setTextColor(TFT_WHITE);
    Cardputer.Display.setTextDatum(MC_DATUM);
    Cardputer.Display.drawString("Scanning...", w / 2, h / 2);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    n = WiFi.scanNetworks(false, true);
    scanned = true;
    currNetwork = 0;
    state = BROWSING;
    needsRedraw = true;
    return;
  }

  // STATE: BROWSING NETWORKS
  if (state == BROWSING) {
    // Handle rescan
    if (isKeyPressed('r') || isKeyPressed('R')) {
      state = SCANNING;
      scanned = false;
      currNetwork = 0;
      needsRedraw = true;
      return;
    }

    // Handle navigation
    if (isKeyPressed('/')) {
      currNetwork++;
      if (currNetwork >= n) {
        currNetwork = 0;
      }
      needsRedraw = true;
    }

    if (isKeyPressed(',')) {
      currNetwork--;
      if (currNetwork < 0) {
        currNetwork = n - 1;
      }
      needsRedraw = true;
    }

    // Handle selection
    if (isKeyPressed(KEY_ENTER) && n > 0) {
      selectedSSID = WiFi.SSID(currNetwork);
      state = SELECTED;
      needsRedraw = true;
      return;
    }

    // Draw network list
    if (needsRedraw) {
      clearScrn();
      if (n == 0) {
        Cardputer.Display.setTextDatum(MC_DATUM);
        Cardputer.Display.setTextColor(TFT_WHITE);
        Cardputer.Display.drawString("no networks found", w / 2, h / 2);
      } else {
        String ssid = WiFi.SSID(currNetwork);
        int32_t rssi = WiFi.RSSI(currNetwork);
        wifi_auth_mode_t encryption = WiFi.encryptionType(currNetwork);

        String securityType;
        if (encryption == WIFI_AUTH_OPEN) {
          securityType = "Open";
        } else if (encryption == WIFI_AUTH_WEP) {
          securityType = "WEP";
        } else if (encryption == WIFI_AUTH_WPA_PSK) {
          securityType = "WPA";
        } else if (encryption == WIFI_AUTH_WPA2_PSK) {
          securityType = "WPA2";
        } else if (encryption == WIFI_AUTH_WPA_WPA2_PSK) {
          securityType = "WPA/WPA2";
        } else {
          securityType = "WPA3";
        }

        String strength = String(rssi) + " dBm";
        drawBattery();
        Cardputer.Display.setTextColor(TFT_WHITE);
        Cardputer.Display.setTextDatum(MC_DATUM);

        if (M5Cardputer.Display.textWidth(ssid) > w - 2) {
          int len = ssid.length();
          int mid = len / 2;
          String top = ssid.substring(0, mid);
          String bottom = ssid.substring(mid);
          Cardputer.Display.drawString(top, w / 2, h / 2 - 40);
          Cardputer.Display.drawString(bottom, w / 2, h / 2 - 20);
        } else {
          Cardputer.Display.drawString(ssid, w / 2, h / 2 - 20);
        }

        Cardputer.Display.setTextColor(TFT_CYAN);
        Cardputer.Display.drawString(strength, w / 2, h / 2);
        Cardputer.Display.setTextColor(TFT_MAGENTA);
        Cardputer.Display.drawString(securityType, w / 2, h / 2 + 20);

        int dotY = h - 20;
        int dotSpacing = 15;
        int startX = (w / 2) - ((n - 1) * dotSpacing / 2);
        if (n > 10) {
          startX = (w / 2) - (9 * dotSpacing / 2);
        }

        int dotsToShow = n > 10 ? 10 : n;
        for (int i = 0; i < dotsToShow; i++) {
          int dotX = startX + (i * dotSpacing);
          uint16_t color;
          if (i == currNetwork) {
            color = TFT_PURPLE;
            Cardputer.Display.fillCircle(dotX, dotY, 4, color);
          } else {
            color = TFT_DARKGREY;
            Cardputer.Display.fillCircle(dotX, dotY, 2, color);
          }
        }
      }
      needsRedraw = false;
    }
  }
}

void resetWiFiScanState() {
  scanned = false;
  n = 0;
  currNetwork = 0;
}

void handleOptions() {}

// DATUM OPTIONS
// TL_DATUM - Top Left (x=0, y=0)
// TC_DATUM - Top Center (x=w/2, y=0)
// TR_DATUM - Top Right (x=w, y=0)
// ML_DATUM - Middle Left (x=0, y=h/2)
// MC_DATUM - Middle Center (x=w/2, y=h/2)
// MR_DATUM - Middle Right (x=w, y=h/2)
// BL_DATUM - Bottom Left (x=0, y=h)
// BC_DATUM - Bottom Center (x=w/2, y=h)
// BR_DATUM - Bottom Right (x=w, y=h)