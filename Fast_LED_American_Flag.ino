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

// Create an array of colors to quickly access individual colors or create cycles
CHSV COLORS_CHSV[] =  {CHSV(0, 255, 255),    // Red
                      CHSV(0, 0, 255),      // White
                      CHSV(170, 255, 255)   // Purple
};

CRGB COLORS_CRGB[] =  {CRGB(255, 0, 0),    // Red
                      CRGB(255, 255, 255),      // White
                      CRGB(0, 0, 255)   // Purple
};

// Or we can set these colors to a custom gradient palette
// Here are the same colors as ealier as expressed in RGB format
// Notice how I repeated the fist and last colors to extend them into the palette

DEFINE_GRADIENT_PALETTE( PALETTE ) {
        0,    255, 0,   0,   // First color repeats
        21,   255, 0,   0,   // Red
        128,   255, 255, 255,   // White
        233,   0, 0, 255,   // Blue
        255,   0, 0,   255
};

// Then we can create a palette to use later
// (note this can be buried further and destroyed after use to save memory)
CRGBPalette16 rainbowPalette = PALETTE;

// You can use the whole strip or select from a section
// This array holds the first and last index for each section of the strip
int STRIPS [][2] = {
        // Longer strips
        {0, 41},      // 0  First strip on left - runs down
        {42, 83},     // 1  Second strip on left - runs up
        {84, 125},    // 2  Third strip on left - runs down

        // Shorter strips
        {126, 155},   // 3  Fourth strip from right - runs down
        {156, 185},   // 4  Third strip from right - runs down
        {186, 215},   // 5  Second strip from right - runs down
        {216, 245},   // 6  First strip from right - runs down

        // Matrix
        {246, 251},   // 7  Row one - runs left
        {252, 257},   // 8  Row two - runs right
        {258, 263},   // 9  Row three - runs left
        {264, 269},   // 10 Row four - runs right
        {270, 275},   // 11 Row five - runs left
        {276, 281},   // 12 Row six - runs right

        // "Ring"
        {282, 286},   // 13 Top - runs left
        {287, 289},   // 14 Left - runs down
        {290, 294},   // 15 Bottom - runs right
        {295, 297}    // 16 Right - runs up
};

// Finally create a CRGB object to hold all of the leds in our strip
CRGB leds[NUM_LEDS];

//////////////////////////////////////////////
////////////      Setup       ////////////////
//////////////////////////////////////////////

// The setup is fairly standard
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
  // put your main code here, to run repeatedly:

  wipeMatrixVertical(COLORS_CRGB[0], 0, 0, 100);
  wipeMatrixVertical(COLORS_CRGB[1], 1, 1, 100);
  wipeMatrixVertical(COLORS_CRGB[0], 2, 2, 100);
  wipeMatrixVertical(COLORS_CRGB[1], 3, 3, 100);
  wipeMatrixVertical(COLORS_CRGB[0], 4, 4, 100);
  wipeMatrixVertical(COLORS_CRGB[1], 5, 5, 100);
  wipeMatrixVertical(COLORS_CRGB[0], 6, 6, 100);

  // Capture the start time
  unsigned long start_time = millis();

  int duration = 5000;

  while(millis() < start_time + duration){

    int random_index = (rand() % (297 - 246)) + 246;

    leds[random_index] = COLORS_CRGB[1];

    blendWithColor(COLORS_CRGB[2], 246, 297, 20);
    
  }

  cleanUpLEDs(0);


}

void blendWithColor(CRGB color, int start_led, int end_led, int steps){
  for(int i=start_led; i<end_led + 1; i++){
    leds[i] = blend(leds[i], color, steps);
  }
  FastLED.show();
}


// Wipes the matrix row by row with a single color
void wipeMatrixVertical(CRGB color, int start_strip, int end_strip, int wait){

    // Setup some variables to hold the range to adjust
    int first_led, last_led;

    // Loop through the strips and set all leds to a value using gradient
    for(int i=start_strip; i<end_strip + 1; i++){

            // Capture the range for this strip
            first_led = STRIPS[i][0];
            last_led = STRIPS[i][1];

            // Set all pixels in range to the color
            fill_gradient_RGB (leds, first_led, color, last_led, color);

            // As always show the leds
            FastLED.show();

            // Wait for a bit
            delay(wait);
        }
}


// Helper function to fade everything to black
void cleanUpLEDs(int wait){
    // Simple animation wait then fade out all leds
    delay(wait);
    for(int i=0; i<50; i++){

        fadeToBlackBy(leds, NUM_LEDS, 20);
        FastLED.show();
        delay(10);

    }
}
