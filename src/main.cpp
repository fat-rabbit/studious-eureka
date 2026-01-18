#include <Arduino.h>

enum BlinkMode
{
  ModeFast,
  ModeSlow
};

const unsigned long DEBOUNCE_DELAY = 50;

unsigned long lastChangeTime = 0;
unsigned long lastDebounceTime = 0;
uint16_t intervalMs = 300;
BlinkMode currentMode = ModeFast;

const uint8_t RED_LED_PIN = 6;
const uint8_t BLUE_LED_PIN = 41;
const uint8_t BUTTON_PIN = 3;
const uint8_t BOOT_BUTTON_PIN = 0;

uint8_t lastButtonState = HIGH;
uint8_t lastBootButtonState = HIGH;

void setup()
{
  // Serial.begin(9600);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  unsigned long currentTime = millis();

  int currentButton = digitalRead(BUTTON_PIN);
  int currentBoot = digitalRead(BOOT_BUTTON_PIN);

  if (currentButton == LOW && lastButtonState == HIGH)
  {
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
    {
      currentMode = ModeFast;
      intervalMs = 300;
      lastDebounceTime = currentTime;
    }
  }
  lastButtonState = currentButton;

  if (currentBoot == LOW && lastBootButtonState == HIGH)
  {
    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
    {
      currentMode = ModeSlow;
      intervalMs = 900;
      lastDebounceTime = currentTime;
    }
  }
  lastBootButtonState = currentBoot;

  if (currentTime - lastChangeTime >= intervalMs)
  {
    lastChangeTime = currentTime;

    bool isRedOn = digitalRead(RED_LED_PIN);
    digitalWrite(RED_LED_PIN, !isRedOn);
    digitalWrite(BLUE_LED_PIN, isRedOn);
  }
}