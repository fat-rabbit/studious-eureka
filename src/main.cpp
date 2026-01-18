#include <Arduino.h>
#include <morse.h>

constexpr uint8_t DEBUG_LED_PIN = 16;
constexpr uint8_t BUTTON_PIN = 3;
constexpr uint32_t INTERVAL_MS = 500;
constexpr uint32_t DEBOUNCE_MS = 50;

int8_t *phraseCursor = nullptr;
int8_t *phraseStart = nullptr;
uint32_t lastChangeTime = 0;
uint32_t currentDelayMs = 100;
uint16_t totalSignals = 0;
uint16_t remainingSignals = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(DEBUG_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  totalSignals = toMorse("Hello, World!", phraseStart);
  phraseCursor = phraseStart;
  remainingSignals = totalSignals;
}

void loop()
{
  const uint32_t currentTime = millis();

  static uint32_t lastDebounceTime = 0;
  static uint8_t lastButtonState = HIGH;

  const uint8_t currentButton = digitalRead(BUTTON_PIN);
  if (currentButton != lastButtonState)
  {
    if (currentTime - lastDebounceTime > DEBOUNCE_MS)
    {
      if (currentButton == LOW)
      {
        // Start transmitting over
        phraseCursor = phraseStart;
        remainingSignals = totalSignals;
        currentDelayMs = 0;
      }
      lastDebounceTime = currentTime;
      lastButtonState = currentButton;
    }
  }

  if (remainingSignals > 0 && (currentTime - lastChangeTime >= currentDelayMs))
  {
    const int8_t signalTU = *phraseCursor;

    if (signalTU > 0)
    {
      digitalWrite(DEBUG_LED_PIN, HIGH);
      currentDelayMs = (uint32_t)signalTU * INTERVAL_MS;
    }
    else
    {
      digitalWrite(DEBUG_LED_PIN, LOW);
      currentDelayMs = (uint32_t)abs(signalTU) * INTERVAL_MS;
    }

    lastChangeTime = currentTime;
    phraseCursor++;
    remainingSignals--;

    if (remainingSignals == 0)
      digitalWrite(DEBUG_LED_PIN, LOW);
  }
}