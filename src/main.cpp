#include "BLEAdvertisedDevice.h"
#include "BLEDevice.h"
#include "BLEScan.h"
#include "BLEUtils.h"
#include "M5Cardputer.h"
#include "WiFi.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "globals.h"
#include "input.h"
#include "ui.h"

decltype(M5Cardputer) &Cardputer = M5Cardputer;
decltype(M5Cardputer.Keyboard) *Keyboard = &Cardputer.Keyboard;

String selection = "";

void setup() {
  auto cfg = M5.config();
  Serial.begin(11520);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  esp_err_t res = esp_wifi_set_promiscuous(true);
  if (res == ESP_OK) {
    Serial.println("promiscious mode enabvled");
  } else {
    Serial.println("couldn't enable promiscious mode");
  }

  Cardputer.begin(cfg, true);
  int textsize = Cardputer.Display.height() / 60;
  if (textsize == 0)
    textsize = 1;
  Cardputer.Display.setTextSize(textsize); // scales both X and Y
  Cardputer.Display.setTextColor(TFT_VIOLET);
  Cardputer.Display.fillScreen(TFT_BLACK);
  drawMenu();
}

void loop() {
  Cardputer.update();

  if (Cardputer.Keyboard.isChange()) {
    handleInput();
  }
}