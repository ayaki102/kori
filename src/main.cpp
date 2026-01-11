#include "BLEAdvertisedDevice.h"
#include "BLEDevice.h"
#include "BLEScan.h"
#include "BLEUtils.h"
#include "M5Cardputer.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"

decltype(M5Cardputer) &Cardputer = M5Cardputer;
decltype(M5Cardputer.Keyboard) *Keyboard = &Cardputer.Keyboard;

const char *menuItems[] = {"WiFi Scan", "Deautoryzacja", "BLE skan", "Opcje"};
const int menuItemsCount = sizeof(menuItems) / sizeof(menuItems[0]);
int selectedItem = 0;
int currentMenuIndex = 0;
bool isInSubMenu = false;

esp_err_t res;

// DECLAERATIONS
void drawMenu();

//

void setup() {
  auto cfg = M5.config();
  Serial.begin(11520);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  esp_err_t res = esp_wifi_set_promiscuous(true);

  Cardputer.begin(cfg, true);
  int textsize = Cardputer.Display.height() / 60;
  if (textsize == 0)
    textsize = 1;
  Cardputer.Display.setTextSize(textsize); // scales both X and Y
  Cardputer.Display.fillScreen(TFT_BLACK);

  drawMenu();
}

void loop() {
  Cardputer.update();
  if (Cardputer.Keyboard.isChange()) {
  }
}

void drawMenu() {}