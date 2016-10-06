#define FOCUS_INPUT_PIN 4
#define FLASH_INPUT_PIN 3
#define FLASH_OUTPUT_PIN 13
#define RING_PIN 11

boolean focusState = LOW;
float ringBrightness = 0;
float fadeSpeed = 0.05;

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

  digitalWrite(FLASH_OUTPUT_PIN, HIGH);
}

void loop() {
  Serial.println(lightingState);


  switch (lightingState) {

    case IDLE_STATE:

      if (digitalRead(FOCUS_INPUT_PIN)) {
        lightingState = FOCUS_STATE;
        ringBrightness = 0;
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
      digitalWrite(FLASH_OUTPUT_PIN, LOW);
      delay(400);
      digitalWrite(FLASH_OUTPUT_PIN, HIGH);
      lightingState = IDLE_STATE;
      delay(4000); // wait for focus signal to go LOW
      break;
  }
}



