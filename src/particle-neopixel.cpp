/**
 * This is a minimal example, see extra-examples.cpp for a version
 * with more explantory documentation, example routines, how to
 * hook up your pixels and all of the pixel types that are supported.
 *
 * On Photon, Electron, P1, Core and Duo, any pin can be used for Neopixel.
 *
 * On the Argon, Boron and Xenon, only these pins can be used for Neopixel:
 * - D2, D3, A4, A5
 * - D4, D6, D7, D8
 * - A0, A1, A2, A3
 *
 * In addition on the Argon/Boron/Xenon, only one pin per group can be used at a
 * time. So it's OK to have one Adafruit_NeoPixel instance on pin D2 and another
 * one on pin A2, but it's not possible to have one on pin A0 and another one on
 * pin A1.
 */

#include "ArduinoJson.h"
#include "Particle.h"
#include "neopixel.h"

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 36
#define PIXEL_TYPE 0x02  // WS2812B

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

struct Config {
  uint8_t version;
  uint8_t delayMs;
  uint8_t brightness;
  bool enabled;
};

Config config;
DynamicJsonDocument doc(JSON_OBJECT_SIZE(3));
char settingsJson[128];

// Prototypes for local build, ok to leave in for Build IDE
void rainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);

size_t serializeSettings() {
  doc["delay"] = config.delayMs;
  doc["brightness"] = config.brightness;
  doc["enabled"] = config.enabled;
  return serializeJson(doc, settingsJson);
}

void writeEEPROM() {
  EEPROM.put(0, config);
  serializeSettings();
}

int enable(String ignored) {
  if (!config.enabled) {
    config.enabled = true;
    writeEEPROM();
    return 1;
  }
  return 0;
}

int disable(String ignored) {
  if (config.enabled) {
    config.enabled = false;
    strip.setBrightness(0);
    strip.show();
    writeEEPROM();
    return 1;
  }
  return 0;
}

int setBrightness(String value) {
  uint8_t brightness = value.toInt();
  if (brightness == 0) {
    return disable("");
  }
  if (brightness <= 100 && brightness != config.brightness) {
    config.brightness = brightness;
    writeEEPROM();
    return 1;
  }
  return 0;
}

int setDelay(String value) {
  uint8_t delayMs = value.toInt();
  if (delayMs > 0 && delayMs != config.delayMs) {
    config.delayMs = delayMs;
    writeEEPROM();
    return 1;
  }
  return 0;
}

void setup() {
  EEPROM.get(0, config);
  if (config.version != 0) {
    Config defaultConfig = {0, 100, 50, true};
    config = defaultConfig;
    writeEEPROM();
  }

  serializeSettings();

  strip.begin();
  strip.show();  // Initialize all pixels to 'off'

  Particle.function("enable", enable);
  Particle.function("disable", disable);
  Particle.function("setBrightness", setBrightness);
  Particle.function("setDelay", setDelay);

  Particle.variable("config", settingsJson);
}

void loop() {
  if (config.enabled) {
    rainbow(config.delayMs);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  strip.setBrightness(config.brightness);
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
