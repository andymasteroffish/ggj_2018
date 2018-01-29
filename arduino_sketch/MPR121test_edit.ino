/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

#include <Wire.h>
#include "Adafruit_MPR121.h"

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  
  //Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    //Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  //Serial.println("MPR121 found!");
}

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();

  
  for (uint8_t i=0; i<12; i++) {

    char touchChar = 'x';
    switch (i) {
      case 0:
        touchChar = '0';
        break;
      case 1:
        touchChar = '1';
        break;
      case 2:
        touchChar = '2';
        break;
      case 3:
        touchChar = '3';
        break;
      case 4:
        touchChar = '4';
        break;
      case 5:
        touchChar = '5';
        break;
      case 6:
        touchChar = '6';
        break;
      case 7:
        touchChar = '7';
        break;
      case 8:
        touchChar = '8';
        break;
      case 9:
        touchChar = '9';
        break;
      case 10:
        touchChar = 'a';
        break;
      case 11:
        touchChar = 'b';
        break;
      default:
        touchChar = 'x';
    }

    char relChar = 'x';
    switch (i) {
      case 0:
        relChar = 'q';
        break;
      case 1:
        relChar = 'w';
        break;
      case 2:
        relChar = 'e';
        break;
      case 3:
        relChar = 'r';
        break;
      case 4:
        relChar = 't';
        break;
      case 5:
        relChar = 'y';
        break;
      case 6:
        relChar = 'u';
        break;
      case 7:
        relChar = 'i';
        break;
      case 8:
        relChar = 'o';
        break;
      case 9:
        relChar = 'p';
        break;
      case 10:
        relChar = 'k';
        break;
      case 11:
        relChar = 'l';
        break;
      default:
        relChar = 'x';
    }

    
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
       //Serial.print('i'); Serial.print(idChar); Serial.print("t");
        Serial.print(touchChar);
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
       //Serial.print('i'); Serial.print(idChar); Serial.print("r");
        Serial.print(relChar);
    }
  }

  // reset our state
  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;
  
  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();
  
  // put a delay so it isn't overwhelming
  delay(100);
}
