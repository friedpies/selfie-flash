/*************************************************** 
  This is an example for our Adafruit 12-channel PWM/LED driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/

  These drivers uses SPI to communicate, 2 pins are required to  
  interface: Data and Clock. The boards are chainable

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_TLC59711.h"
#include <SPI.h>

// How many boards do you have chained?
#define NUM_TLC59711 1

#define data   12
#define clock  13

Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711, clock, data);
//Adafruit_TLC59711 tlc = Adafruit_TLC59711(NUM_TLC59711);

void setup() {
  Serial.begin(9600);
  
  Serial.println("TLC59711 test");
  pinMode(10, OUTPUT);
  tlc.begin();
  tlc.write();
}

void loop() {
  Serial.println("TEST");
  tlc.setLED(1, 65535, 65535, 65535);
}


// Fill the dots one after the other with a color
void colorWipe(uint16_t r, uint16_t g, uint16_t b, uint8_t wait) {
  for(uint16_t i=0; i<8*NUM_TLC59711; i++) {
      tlc.setLED(i, r, g, b);
      tlc.write();
      delay(wait);
  }
}

