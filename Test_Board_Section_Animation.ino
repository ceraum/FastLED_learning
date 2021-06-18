// Created by CE Raum June 18, 2021
// A test structure for using the LED Panel Test Board
// This sketch uses mostly FOR and WHILE loops to create animations of sections of one strip of addressable LEDs
// Note that these kinds of animations are "blocking" code
// In other words, while the animation is running it will be the only one
// However the ring portion uses an example of the built in EVERY_N_MILLISECONDS and wave functions
// The structure is verbose in an effort to make it easier to read and use the test board for building your own animations
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

// Create an array of colors to quickly access individual colors or create cycles
CHSV COLORS[] =  {CHSV(0, 255, 255),    // Red
                  CHSV(24, 255, 255),   // Orange
                  CHSV(43, 255, 255),   // Yellow
                  CHSV(93, 255, 255),   // Green
                  CHSV(170, 255, 255),  // Blue
                  CHSV(215, 255, 255)   // Purple
}; // Purple

// Or we can set these colors to a custom gradient palette
// Here are the same colors as ealier as expressed in RGB format
// Notice how I repeated the fist and last colors to extend them into the palette

DEFINE_GRADIENT_PALETTE( PALETTE ) {
        0,    255, 0,   0,   // First color repeats
        21,   255, 0,   0,   // Red
        64,   255, 165, 0,   // Orange
        106,   251, 255, 0,   // Yellow
        149,   0,   255, 47,   // Green
        191,   0,   0,   255,  // Blue
        233,   255, 0,   242,  // Purple
        255,   255, 0,   242   // Last color repeated
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

//////////////////////////////////////////////
////////////    MAIN LOOP     ////////////////
//////////////////////////////////////////////
void loop() {

    // It is handy to setup some on/off switches for testing

    bool testFirstSection = true;
    bool test_gradient_wipe = true;
    bool testFadeOut = true;
    bool test_matrix = true;
    bool test_ring = true;

    // Because all of our colors and section are easily selectable we can cycle through them to set values
    // Let's wipe the first six sections with the six colors
    if(testFirstSection){

        for(int i=0; i<6; i++) {
            // Running the standard color wipe routine
            // input a color, the start and stop leds (indexes 0 and 1) and a delay
            colorWipeHSV(COLORS[i], STRIPS[i][0], STRIPS[i][1], 2);

        }
    }


    // We can wipe a section using our custom palette
    if(test_gradient_wipe){

        // Here we will send the custom palette created earlier and start/stop of the seventh strip
        gradientWipe(rainbowPalette, STRIPS[6][0], STRIPS[6][1], 2);

    }

    // Use a while loop to continue a process until it is finished
    if(testFadeOut){
        
        // Here we will apply a gradual fadeout to a number of strips
        // The fade is hardcoded and not fast (fadeToBlackBy is much better)
        // But this allows us to fade sections and test conditions
        fadeOutSections(0, 6);  //
    }

    // A matrix presents challenges because the rows tend to run alternating right and left
    // However, since we defined each row's start and finish led we can iterate through it easily in any direction
    if(test_matrix){
        
        // We can reuse the gradient color wipe to fill the matrix to see the direction
        gradientWipe(rainbowPalette, STRIPS[7][0], STRIPS[12][1], 20);

        // Now lets erase the colors off vertically
        // We can use a trick to quickly emulate the fill_color function with fill_gradient
        // Here I will fill a section with black
        wipeMatrixVertical(CRGB (0, 0, 0), 7, 12, 100);

        // Or we can create a custom animation that deals with columns as if they were rows
        wipeMatrixHorizontal(CRGB (0, 0, 0), 7, 12, 100);

        // A helper function to fade everything to black after a delay
        cleanUpLEDs(2000);

    }

    // In some applications you may have strings where the start and end meet - but rings will always do so
    // Here we use one of the built in wave functions to animate six dots
    if(test_ring){

        running_circles(282, 297, 50, 4000);

        cleanUpLEDs(2000);
    }

}
//////////////////////////////////////////////
//////////// END OF MAIN LOOP ////////////////
//////////////////////////////////////////////

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

// Wipes a section of the strip with an indexed color
void colorWipeHSV(CHSV color, int start_led, int end_led, int wait) {

    // This is the most basic of FastLED routines
    // Simply iterate though all the leds in a range and set to a color
    for(int i=start_led; i<end_led + 1; i++) {  // For each led in strip...
        leds[i] = color;                        // Set the led to a color
        FastLED.show();                         // Show the leds
        delay(wait);                           //  Pause for a moment
    }

}

// Wipes a section of the strip with a defined gradient
void gradientWipe(CRGBPalette16 myPal, int start_led, int end_led, int wait) {

    for(int i=start_led; i<end_led + 1; i++) { // For each pixel in strip...

        // The arduino map function works well to select an index from 0-255 scaled to the number of pixels
        // note that I have selected a subrange of the colors (indexes 10 to 245) to smooth the start and finish
        // This is adjustable and required the copying of the start and finish colors when defining the gradient
        // it also avoids a FastLED bug when selecting index 255

        // Create the index of the gradient by mapping the led index to the range of colors on the gradient
        uint8_t color_index = map(i, start_led, end_led, 10, 245);

        // Set the color of the led to the color generated by the gradient at that index
        leds[i] = ColorFromPalette( myPal, color_index, BRIGHTNESS, LINEARBLEND); // normal palette access

        // For testing and also to slow it down (because sometimes it misses colors - I dunno)
        // TODO: there must be a cleaner way to show the RGB
        Serial.print(String(i) +  " - " + String(leds[i].red) + ", " + String(leds[i].green) +  + ", " + String(leds[i].blue) + "\n");

        // Show the led strip
        FastLED.show();

        // Wait a bit
        delay(wait);

    }
}

// Fades out entire sections at slightly different speeds
void fadeOutSections(int start_strip, int end_strip){

    // Set a starting assumption that some leds are lit
    bool some_leds_lit = true;

    int current_strip;    // the current index of the strip we are working on
    int current_light;    // the current index of the individual led

    // Loop until all the leds are black/off
    while(some_leds_lit){

        // Immediately set the test value to false - if we find a lit led turn it back to true
        some_leds_lit = false;

        // Set the starting strip index to the first strip
        current_strip = start_strip;

        // Loop until we reach the end strip
        while(current_strip < end_strip + 1) {

            // Set the starting led index to the first led
            current_light = STRIPS[current_strip][0];

            // Loop until we reach the end of the section
            while(current_light < STRIPS[current_strip][1] + 1){

                // Scale the led using nscale8 offsetting by the strip value
                // Adjust this for speeds
                // TODO - this should be parametrized and extended
                leds[current_light].nscale8(252-(current_strip/2));

                // Test if the led is black
                // Any non-zero value evaluates as true
                if(leds[current_light]) {
                    some_leds_lit = true;
                }

                // Increment the current led index
                current_light += 1;

            }

            // Increment the current strip index
            current_strip += 1;

        }

        // Show the leds when all the strips are adjusted
        FastLED.show();

    }

}

// Wipes the matrix column by column with indexed colors
void wipeMatrixHorizontal(CRGB color, int start_strip, int end_strip, int wait){

    // Setup some variables
    int columns = STRIPS[start_strip][1] - STRIPS[start_strip][0];        // How many leds wide (columns)
    int rows = end_strip - start_strip;           // How many rows in the matrix

    // Set the first led's offset from zero
    int row_offset = STRIPS[start_strip];

    // Loop through the columns and set all leds to a value using the indexed color
    for(int col=STRIPS[start_strip][0]; col<STRIPS[start_strip][1] + 1; col++){

        for(int row=start_strip; row<end_strip + 1; row++){

            // For clarity lets set the starting and ending led ranges
            int sample_range_start = STRIPS[start_strip][0];
            int sample_range_end =  STRIPS[start_strip][1];
            int map_range_start = STRIPS[row][0];
            int map_range_end = STRIPS[row][1];

            // Variable to hold the led index that we will want to change after mapping
            int index_to_change;

            // Let's select a color based on the column
            int color_index = (col - STRIPS[start_strip][0]);
            CHSV color = COLORS[color_index];

            // Now if the strip is even we can map the index forward or if odd reverse the map range
            if ( row % 2 == 0){
                index_to_change = map(col, sample_range_start, sample_range_end, map_range_start, map_range_end);

            } else {
                index_to_change = map(col, sample_range_start, sample_range_end, map_range_end, map_range_start);

            }

            // Set the color of this led
            leds[index_to_change] = color;

        }

        // As always, show the leds and wait
        FastLED.show();
        delay(wait);

    }

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

// Creates six colored 'balls" and animates them around the ring using the beat8 function to set the position
void running_circles(int start_led, int end_led, int fade_speed, int duration){

    // Capture the start time
    unsigned long start_time = millis();

    // Set the number of balls to circulate
    int balls = 6;

    // Set how many leds in the "ring"
    int leds_in_ring = end_led - start_led + 1;

    // Run the animation for a set amount of time
    while(millis() < start_time + duration){

        // Loop through the balls
        for(int i=0; i<balls; i++){

            // Use the beat8 function to generate a position along the ring (sawtooth wave)
            uint16_t pos = beat8(i + 1) % leds_in_ring;  // modulo the position to be within NUM_LED

            // Set the led at that position (offset by the starting led number) to the correct color
            leds[start_led + pos] = COLORS[i];
        }

        // And, of course, show the leds
        FastLED.show();

        // We have now come to the phase where delays are less and less useful - next step is to create non-blocking animations
        EVERY_N_MILLISECONDS(fade_speed) {
            // Use a fade to black to clear out the old colored leds
            fadeToBlackBy(leds, NUM_LEDS, 80);

            FastLED.show();
        }

    }



}
