// Created by CE Raum on 7/28/21.
//
// This sketch demonstrates the minimum procedure to turn on a single light in the LED strip
// It includes all of the basic setup required to define and address the LED strip and individual lights
// Enjoy!!

// Always include the FastLED library
#include "FastLED.h"

// At minimum you must tell the library the number of LEDs and the type.
// Also define the data pin attached to the Arduino
// Using defines saves the data during compile - freeing RAM for use later
#define NUM_LEDS 298
#define DATA_PIN 11
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

// You can also set some default values to utilize later during setup or in animations
#define BRIGHTNESS          40
#define FRAMES_PER_SECOND  120

// Finally create a CRGB object to hold all of the leds in our strip
CRGB leds[NUM_LEDS];

//////////////////////////////////////////////
////////////      Setup       ////////////////
//////////////////////////////////////////////

// The setup is fairly standard
// Add any additional code as required
void setup() {

    delay(2000); // power-up safety delay
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5,1500);
    // I'm not sure if this is required, but 13 is the typical on-board led
    set_max_power_indicator_LED(13);
    // Initialize serial port for debugging.
    Serial.begin(115200);
    // Add any code your sketch requires...

}

void loop() {

    // First we will create a color in RGB format
    CRGB this_color = (74, 189, 224);   // This is close to the blue of the MakerBarn logo

    // Then we select the index of the light to change
    int this_index = 1;     // note that the index range starts at 0, so this is the second light

    // Finally call the function to set this light to the given color
    // This code could be in the main loop, but it is helpful and more reusable
    // to call the code separately
    turnOnOneLight(this_color, this_index);

    // The light should now be turned on

}

// Change the color of one pixel
void turnOnOneLight(CRGB color, int index){
    // Simple function to change the color of a LED and display the strip
    // Input an RGB color and the index of the light to change

    // Change the color of the led at this index
    leds[index] = color;

    // Always show the LEDs to see the change
    FastLED.show();

}