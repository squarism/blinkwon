// This #include statement was automatically added by the Particle IDE.
#include <SparkJson.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// Hey!  Particle IDE put these include lines in but you still have to to do the "Add to App" thing.
#include <SparkJson.h>
#include <neopixel.h>
#define PIXEL_TYPE WS2812B


// ---- CHANGE THESE DEPENDING ON YOUR WIRING AND NEOPIXEL HARDWARE ----
#define PIN D6
#define LEDS 24


// Globals of sorts
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, PIXEL_TYPE);
int colorStart = 0;
int colorEnd = 255;
int colorRotationSlowness = defaultColorRotationSpeed();
int brightness = defaultBrightness();

char *theme = "unknown";

uint32_t white = strip.Color(255, 255, 255);
uint32_t black = strip.Color(0, 0, 0);
uint32_t green = strip.Color(45, 210, 0);
uint32_t red = strip.Color(240, 15, 0);
uint32_t lime = strip.Color(105, 150, 80);
uint32_t orange = strip.Color(180, 75, 0);
uint32_t blacklight = strip.Color(105, 0, 150);
uint32_t yellow = strip.Color(150, 105, 0);
uint32_t blue = strip.Color(30, 0, 225);
uint32_t bluegreen = strip.Color(0, 90, 165);
uint32_t pink = strip.Color(230, 0, 25);

void setup() {
    strip.begin();
    strip.setBrightness(brightness);
    strip.show(); // Initialize all pixels to 'off'
    
    // subscribe to private events - this finally seems to work like I want.
    Particle.subscribe("squarism/blinkwon", eventHandler, MY_DEVICES);
}

void loop() {
    // we don't need to call process or connect here
    // the particle defaults to automatic mode.
    // more here about it: http://blog.particle.io/2014/08/06/control-the-connection/
}

int defaultColorRotationSpeed() {
    return 6000;
}

int defaultBrightness() {
    return 120;
}

void eventHandler(const char * event, const char * data) {
    int len = strlen(data);
    char dataCopy[len + 1];
    strcpy(dataCopy, data);

    StaticJsonBuffer<500> jsonBuffer;
    JsonObject & json = jsonBuffer.parseObject(dataCopy);

    if (json.success()) {
        if (json.containsKey("brightness")) {
            int oldBrightness = brightness;
            brightness = json["brightness"];
            tweenToBrightness(oldBrightness, (int)brightness, 90);
        }

        if (json.containsKey("theme")) {
            theme = const_cast <char*> (json["theme"].asString());
            if (strcmp(theme, "off") == 0) {
                tweenToBrightness((int)brightness, 0, 90);
                strip.setBrightness(0);
                strip.show();
                brightness = 0;
            } else {
                setColor(theme);
            }
        }

        if (json.containsKey("idle")) {
            theme = "unknown";
        }
    }
}

void setColor(const char * theme) {
    colorRotationSlowness = defaultColorRotationSpeed();

    // wow, you can't easily do switch statements in C?
    if (strcmp(theme, "bluegreen") == 0) {
        colorWipe(bluegreen, 15);
    } else if (strcmp(theme, "white") == 0) {
        colorWipe(white, 15);
    } else if (strcmp(theme, "blacklight") == 0) {
        colorWipe(blacklight, 15);
    } else if (strcmp(theme, "orange") == 0) {
        colorWipe(orange, 15);
    } else if (strcmp(theme, "yellow") == 0) {
        colorWipe(yellow, 15);
    } else if (strcmp(theme, "red") == 0) {
        colorWipe(red, 15);
    } else if (strcmp(theme, "lime") == 0) {
        colorWipe(lime, 15);
    } else if (strcmp(theme, "blue") == 0) {
        colorWipe(blue, 15);
    } else if (strcmp(theme, "green") == 0) {
        colorWipe(green, 15);
    } else if (strcmp(theme, "pink") == 0) {
        colorWipe(pink, 15);
    } else if (strcmp(theme, "rainbow") == 0) {
        colorStart = 0;
        colorEnd = 255;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "cylon") == 0) {
        colorStart = 0;
        colorEnd = 255;
        strip.setBrightness(120);
        dotChase(90, 45, 3);
    }
}

// Really tried to DRY this out with clever code, but couldn't get it.
void tweenToBrightness(int from, int to, uint8_t wait) {
    if (from == to) { return; }

    if (from < to) {
        for (int i = from; i < to; i++) {
            strip.setBrightness(i);
            strip.show();
            delay(wait);
        }
    } else {
        for (int i = from; i > to; i--) {
            strip.setBrightness(i);
            strip.show();
            delay(wait);
        }
    }

    brightness = to;
}

void idle() {
    strip.setBrightness(50);
    dotChase(245, 35, 2);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void colorRing(uint16_t wait) {
    uint16_t i, j;
    for (j = colorStart; j < colorEnd; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(j & 255));
        }
        strip.show();
        delay(wait);
    }
    for (j = colorEnd; j > colorStart; j--) {
        for (i = 0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(j & 255));
        }
        strip.show();
        delay(wait);
    }
}

// dot chase visual effect
void dotChase(uint32_t c, uint8_t wait, int dots) {
    int offset = LEDS / dots; // pixel separation
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
