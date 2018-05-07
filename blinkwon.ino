// Hey!  Particle IDE put these include lines in but you still have to to do the "Add to App" thing.
#include <SparkJson.h>
#include <neopixel.h>

SYSTEM_MODE(SEMI_AUTOMATIC);
#define MAIN_GROUP 0
#define ACCENT_GROUP 1
#define TEST_GROUP 2



// ---- CHANGE THESE DEPENDING ON YOUR WIRING AND NEOPIXEL HARDWARE ----
#define PIXEL_PIN D6

// SK6812RGBW is RGBW
// WS2812B are the other ones you own
// #define PIXEL_TYPE WS2812B
// #define PIXEL_TYPE SK6812RGBW

// int PIXEL_COUNT = 24;
// int PIXEL_COUNT = 60;
// int PIXEL_COUNT = 120;

// int GROUP = ACCENT_GROUP;
// int GROUP = MAIN_GROUP;
// int GROUP = TEST_GROUP;



// Globals of sorts
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
int colorStart = 0;
int colorEnd = 255;

int brightness = defaultBrightness();

char theme[15];

boolean connectToCloud = false;

uint32_t white = strip.Color(100, 100, 100);
uint32_t black = strip.Color(0, 0, 0);
uint32_t green = strip.Color(45, 210, 0);
uint32_t red = strip.Color(240, 15, 0);
uint32_t lime = strip.Color(105, 150, 80);
uint32_t orange = strip.Color(180, 45, 0);
uint32_t blacklight = strip.Color(95, 0, 140);
uint32_t yellow = strip.Color(150, 105, 0);
uint32_t blue = strip.Color(30, 0, 225);
uint32_t bluegreen = strip.Color(0, 110, 165);
uint32_t pink = strip.Color(230, 0, 25);

static uint8_t getRed(uint32_t c) { return (uint8_t)(c >> 16); }
static uint8_t getGreen(uint32_t c) { return (uint8_t)(c >>  8); }
static uint8_t getBlue(uint32_t c) { return (uint8_t)c; }


void setup() {
    strcmp(theme, "unknownTheme");
    
    // careful with OSH Park PCBs - what is the total budget?  80?
    // red and green are swapped, plus this has white
    if (PIXEL_TYPE == SK6812RGBW) {
        white = strip.Color(0, 0, 0, 80);
        blacklight = strip.Color(0, 15, 65, 0);
        green = strip.Color(70, 10, 0, 0);
        red = strip.Color(5, 75, 0, 0);
        lime = strip.Color(70, 25, 5, 0);
        orange = strip.Color(15, 65, 0, 0);
        yellow = strip.Color(25, 55, 0, 0);
        blue = strip.Color(0, 5, 75, 0);
        bluegreen = strip.Color(25, 0, 55, 0);
        pink = strip.Color(0, 70, 10, 0);
    }
    
    strip.begin();
    
    // I don't know why it's so hard to get it to boot to all pixels as off
    strip.show();
    strip.clear();
    strip.show();
    
    strip.setBrightness(brightness);
    
    Particle.connect();

    String eventChannel;
    if (GROUP == MAIN_GROUP) {
        eventChannel = "squarism/blinkwon";
    }
    
    if (GROUP == ACCENT_GROUP) {
        eventChannel = "squarism/ambient_strip";
    }
    
    if (GROUP == TEST_GROUP) {
        eventChannel = "squarism/test";
    }

    
    // subscribe to private events - this finally seems to work like I want.
    Particle.subscribe(eventChannel, eventHandler, MY_DEVICES);
}

void loop() {
    // we don't need to call process or connect here
    // the particle defaults to automatic mode.
    // more here about it: http://blog.particle.io/2014/08/06/control-the-connection/
    Particle.process();
    
    if(connectToCloud && Particle.connected() == false) {
        Particle.connect();
        connectToCloud = false;
    }
    
    if (strcmp(theme, "cylon") == 0) {
        strip.setBrightness(120);
        if (PIXEL_TYPE == SK6812RGBW) {
            dotChase(45, 90, 3);
        } else {
            dotChase(90, 45, 3);
        }
    }
}

void connect() {
    connectToCloud = true;
}

int defaultBrightness() {
    return 80;
}

void eventHandler(const char * event, const char * data) {
    int len = strlen(data);
    char dataCopy[len + 1];
    strcpy(dataCopy, data);

    StaticJsonBuffer<500> jsonBuffer;
    JsonObject & json = jsonBuffer.parseObject(dataCopy);

    if (json.success()) {
        if (json.containsKey("brightness")) {
            // set brightness here later but avoid calling strip.setBrightness()
            // because that causes a jarring flicker effect and it's very hard to blend
            // or fade brightness changes.  Instead we are going to prefer running
            // down the strip and setting a scaled brightness value.  The defaultBrightness
            // will be 255 and the scaling will be scaling/255.
            // Also attempted but given up on is tweening and fading state as an animation of sorts.
            // Tweening to brightness causes threading type problems to solve.
            
            // Careful - The larger strips can't be super bright without crashing because of the PCB boards.
            brightness = (int)json["brightness"];
        }

        if (json.containsKey("theme")) {
            strcpy(theme, json["theme"].asString());
            if (strcmp(theme, "off") == 0) {
                brightness = 0;
                Particle.process();
                blankWipe(100);
                Particle.process();
            } else {
                Particle.process();
                setColor(theme, 100);
                Particle.process();
            }
        }
    }
}

void setColor(const char * theme, int wipeSpeed) {
    // wow, you can't easily do switch statements in C?
    if (strcmp(theme, "bluegreen") == 0) {
        colorWipe(bluegreen, wipeSpeed);
    } else if (strcmp(theme, "white") == 0) {
        colorWipe(white, wipeSpeed);
    } else if (strcmp(theme, "blacklight") == 0) {
        colorWipe(blacklight, wipeSpeed);
    } else if (strcmp(theme, "orange") == 0) {
        colorWipe(orange, wipeSpeed);
    } else if (strcmp(theme, "yellow") == 0) {
        colorWipe(yellow, wipeSpeed);
    } else if (strcmp(theme, "red") == 0) {
        colorWipe(red, wipeSpeed);
    } else if (strcmp(theme, "lime") == 0) {
        colorWipe(lime, wipeSpeed);
    } else if (strcmp(theme, "blue") == 0) {
        colorWipe(blue, wipeSpeed);
    } else if (strcmp(theme, "green") == 0) {
        colorWipe(green, wipeSpeed);
    } else if (strcmp(theme, "pink") == 0) {
        colorWipe(pink, wipeSpeed);
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, uint16_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        // set the pixel based off unpacked color values and a scaled brightness globalset earlier
        if (PIXEL_TYPE == SK6812RGBW) {
            if (color == white) {
                // huh
                strip.setColorScaled(i, 0, 0, 0, 255, (255 * brightness/255));
            } else {
                // strip.setColorScaled(num, red, green, blue, white, scaling);
                strip.setColorScaled(i, getRed(color), getGreen(color), getBlue(color), 0,  brightness);
            }
        } else {
            // wtf - where is this even coming from
            strip.setColorScaled(i, getRed(color), getGreen(color), getBlue(color), brightness);
        }
        // show immediately in the loop to update the pixel strip as the loop runs, this is how we go down the strip
        strip.show();
        Particle.process();
        delay(wait);
    }
}

// Turns all pixels off sequentially
void blankWipe(uint16_t slowness) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        if (PIXEL_TYPE == SK6812RGBW) {
            strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
        } else {
            strip.setPixelColor(i, strip.Color(0, 0, 0));
        }
        strip.show();
        Particle.process();
        delay(slowness);
    }
}


// dot chase visual effect
void dotChase(uint32_t c, uint16_t wait, int dots) {
    int offset = strip.numPixels() / dots; // pixel separation
    uint32_t color = Wheel(c & 255);

    for (int q = 0; q < offset; q++) {
        for (int i = 0; i < strip.numPixels(); i = i + offset) {
            strip.setPixelColor(i + q, color); // turn every third pixel on
        }

        strip.show();
        delay(wait);

        for (int i = 0; i < strip.numPixels(); i = i + offset) {
            strip.setPixelColor(i + q, 0); // turn every n-offset pixel off
        }

    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}