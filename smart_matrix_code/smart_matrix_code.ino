#include <SmartMatrix3.h>
#include <SPI.h>
#include <SD.h>
#include "GIFDecoder.h"


#define ARDUINO_INPUT_PIN 16
#define DISPLAY_TIME_SECONDS 10
#define ENABLE_SCROLLING  1

const int defaultBrightness = 255;

const rgb24 COLOR_BLACK = {
  0, 0, 0
};


boolean arduinoInput = LOW;
long previousMillis = 0;
long currentMillis = 0;
int fadeValue = 0;
long smileScreenDuration = 4000;

/* SmartMatrix configuration and memory allocation */
#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 64;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 2;       // known working: 2-4
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN; // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);    // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);
const uint8_t kScrollingLayerOptions = (SM_SCROLLING_OPTIONS_NONE);


SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);
#if ENABLE_SCROLLING == 1
SMARTMATRIX_ALLOCATE_SCROLLING_LAYER(scrollingLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kScrollingLayerOptions);
#endif

// Chip select for SD card on the SmartMatrix Shield
#define SD_CS 15

#define GIF_DIRECTORY "/gifs/"

int num_files;



enum states {
  IDLE_STATE,
  SMILE_STATE
};


enum states selfieState = IDLE_STATE;

void setup() {
  pinMode(ARDUINO_INPUT_PIN, INPUT_PULLUP);


  setScreenClearCallback(screenClearCallback);
  setUpdateScreenCallback(updateScreenCallback);
  setDrawPixelCallback(drawPixelCallback);

  // Seed the random number generator
  randomSeed(analogRead(14));
  Serial.begin(115200);

  matrix.addLayer(&backgroundLayer);
#if ENABLE_SCROLLING == 1
  matrix.addLayer(&scrollingLayer);
#endif

  matrix.begin();

  backgroundLayer.fillScreen(COLOR_BLACK);
  backgroundLayer.swapBuffers();

  pinMode(SD_CS, OUTPUT);
  if (!SD.begin(SD_CS)) {
#if ENABLE_SCROLLING == 1
    scrollingLayer.start("No SD card", -1);
#endif
    Serial.println("No SD card");
    while (1);
  }
  num_files = enumerateGIFFiles(GIF_DIRECTORY, false);

  if (num_files < 0) {
#if ENABLE_SCROLLING == 1
    scrollingLayer.start("No gifs directory", -1);
#endif
    Serial.println("No gifs directory");
    while (1);
  }

  if (!num_files) {
#if ENABLE_SCROLLING == 1
    scrollingLayer.start("Empty gifs directory", -1);
#endif
    Serial.println("Empty gifs directory");
    while (1);
  }

  processGIFFile("/gifs/selfie.gif");
}


void loop() {
  switch (selfieState) {
    case IDLE_STATE: {

        if (digitalRead(ARDUINO_INPUT_PIN) == LOW) {
          selfieState = SMILE_STATE;
          previousMillis = millis();
          processGIFFile("/gifs/smile.gif");
        }
        break;
      }

    case SMILE_STATE: {
        currentMillis = millis() - previousMillis;
        if (currentMillis > smileScreenDuration) {
          selfieState = IDLE_STATE;
          processGIFFile("/gifs/SELFIE.gif");
          previousMillis = millis();
        }
        break;
      }
  }
}

void screenClearCallback(void) {
  backgroundLayer.fillScreen({0, 0, 0});
}

void updateScreenCallback(void) {
  backgroundLayer.swapBuffers();
}

void drawPixelCallback(int16_t x, int16_t y, uint8_t red, uint8_t green, uint8_t blue) {
  backgroundLayer.drawPixel(x, y, {red, green, blue});
}
