#include <Adafruit_NeoPixel.h>

char inChar = 0;
int flash = LOW;
int flashPin = 2;
int flashOut = 3;
int PIN = 5;
int NUMPIXELS = 60;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(flashPin, INPUT);
  analogWrite(flashOut, 255);
  pixels.begin(); // This initializes the NeoPixel library.
  for (int i = 0; i < NUMPIXELS; i++) {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
  }
  pixels.show();
}

void loop() {
  flash = digitalRead(flashPin);
  inChar = Serial.read();
  Serial.println(inChar);



  //  if (flash) {
  if (inChar == 'g') {
    pixelFadeIn(155);
    delay(50);
    analogWrite(flashOut, 0);
    setAllPixels(0);
    delay(150);
    analogWrite(flashOut, 255);
    setAllPixels(155);
    delay(400);
    pixelFadeOut();
  }
}

void setAllPixels(int value) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(value, value, value)); // Moderately bright green color.
  }
  pixels.show();
}

void pixelFadeIn(int value) {
  for (int j = 0; j < value; j++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(j, j, j)); // Moderately bright green color.
    }
    pixels.show();
    delay(5);
  }
}

void pixelFadeOut(){
  for (int j = 155; j >= 0; j--) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(j, j, j)); // Moderately bright green color.
    }
    pixels.show();
  }
}



