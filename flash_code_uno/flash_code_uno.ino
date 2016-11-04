#define FOCUS_INPUT_PIN 4
#define FLASH_INPUT_PIN 2
#define FLASH_OUTPUT_PIN 5
#define RING_PIN 6
#define TEENSY_PIN 8

boolean focusState = LOW;
int lampBrightness = 175;
float ringBrightness = 0;
float fadeSpeed = 0.02;

enum states {
  IDLE_STATE,
  FOCUS_STATE,
  FLASH_STATE
};

enum states lightingState = IDLE_STATE;

void setup() {
  Serial.begin(115200);
  pinMode(FOCUS_INPUT_PIN, INPUT);
  pinMode(FLASH_INPUT_PIN, INPUT_PULLUP);
  pinMode(FLASH_OUTPUT_PIN, OUTPUT);
  pinMode(RING_PIN, OUTPUT);
  pinMode(TEENSY_PIN, OUTPUT);

  analogWrite(FLASH_OUTPUT_PIN, lampBrightness);
  digitalWrite(RING_PIN, LOW);
  digitalWrite(TEENSY_PIN, HIGH);
}

void loop() {
  switch (lightingState) {

    case IDLE_STATE:

      if (digitalRead(FOCUS_INPUT_PIN)) {
        lightingState = FOCUS_STATE;
        ringBrightness = 0;
        digitalWrite(TEENSY_PIN, LOW);
        delay(100);
        digitalWrite(TEENSY_PIN, HIGH);
      }

      break;

    case FOCUS_STATE:
      analogWrite(RING_PIN, int(ringBrightness));
      ringBrightness += fadeSpeed;
      if (ringBrightness > 255) ringBrightness = 255;

      if (!digitalRead(FLASH_INPUT_PIN)) {
        lightingState = FLASH_STATE;
      }
      if (!digitalRead(FOCUS_INPUT_PIN)) {
        lightingState = IDLE_STATE;
        ringBrightness = 0;
        analogWrite(RING_PIN, int(ringBrightness));
      }

      break;

    case FLASH_STATE:
      delay(100); // wait until camera is finished taking photo
      analogWrite(RING_PIN, 0);
      analogWrite(FLASH_OUTPUT_PIN, 0);
      delay(400);
      analogWrite(FLASH_OUTPUT_PIN, lampBrightness);
      lightingState = IDLE_STATE;
      delay(4000); // wait for focus signal to go LOW
      break;
  }
}



