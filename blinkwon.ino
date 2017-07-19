// This #include statement was automatically added by the Particle IDE.
#include <SparkJson.h>

// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>

// Hey!
// Particle IDE put these include lines in but you still have to to do the "Add to App" thing.
#include <SparkJson.h>
#include <neopixel.h>


// ---- CHANGE THIS ----
#define PIN D6
#define LEDS 24


#define PIXEL_TYPE WS2812B
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, PIXEL_TYPE);

// status light colors
int colorStart = 0;
int colorEnd = 255;
int colorRotationSlowness = defaultColorRotationSpeed();
int brightness = defaultBrightness();

char *theme = "unknown";

uint32_t white = strip.Color(255, 255, 255);
uint32_t black = strip.Color(0, 0, 0);


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

  if (strcmp(theme, "unknown") == 0 ) {
    // idle();
  }
    
  if (strcmp(theme, "cylon") == 0 ) {
    colorStart = 0;
    colorEnd = 255;
    strip.setBrightness(120);
    dotChase(90, 45, 3);
  }
}

int defaultColorRotationSpeed() {
   return 6000; 
}

int defaultBrightness() {
    return 120;
}

void eventHandler(const char *event, const char *data) {
    int len = strlen(data);
    char dataCopy[len+1];
    strcpy(dataCopy, data);


    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(dataCopy);
    
    if (json.success()) {
      if (json.containsKey("brightness"))
      {
        brightness = json["brightness"];
        strip.setBrightness(brightness);
        strip.show();
      }
    
      if ( json.containsKey("theme"))
      {
        theme = const_cast<char*>(json["theme"].asString());
        setColor(theme);
      } 
    
      if (json.containsKey("idle"))
      {
        theme = "unknown";
      }
    }
}

void setColor(const char *theme) {
    colorRotationSlowness = defaultColorRotationSpeed();
    
    // wow, you can't easily do switch statements in C?
    if (strcmp(theme, "green") == 0) {
        colorStart = 15;
        colorEnd = 35;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "lime") == 0) {
        colorStart = 35;
        colorEnd = 45;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "red") == 0) {
        colorStart = 80;
        colorEnd = 95;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "blacklight") == 0 ) {
        colorStart = 135;
        colorEnd = 160;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "yellow") == 0 ) {
        colorStart = 50;
        colorEnd = 70;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "blue") == 0 ) {
        colorStart = 160;
        colorEnd = 180;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "bluegreen") == 0 ) {
        colorStart = 200;
        colorEnd = 220;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "orange") == 0 ) {
        colorStart = 60;
        colorEnd = 80;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "pink") == 0 ) {
        colorStart = 90;
        colorEnd = 110;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "rainbow") == 0 ) {
        colorStart = 0;
        colorEnd = 255;
        colorRing(colorRotationSlowness);
    } else if (strcmp(theme, "off") == 0 ) {
        colorWipe(black, 80);
        strip.setBrightness(0);
        strip.show();
    } else if (strcmp(theme, "white") == 0 ) {
        colorWipe(white, 0);
    }
}

void idle() {
    strip.setBrightness(50);
    dotChase(245, 35, 2);
}

// This doesn't work at all.  Need to guard with a boolean maybe?
void fadeOut(uint8_t wait) {
    for(uint8_t i=brightness; i >= 0; i--) {
        strip.setBrightness(i);
        strip.show();
        delay(wait);
    }
    strip.setBrightness(0);
    strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorRing(uint16_t wait) {
  uint16_t i, j;
  for(j=colorStart; j<colorEnd; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j & 255));
    }
    strip.show();
    delay(wait);
  }
  for(j=colorEnd; j>colorStart; j--) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(j & 255));
    }
    strip.show();
    delay(wait);
  }

}


// dot chase visual effect
void dotChase(uint32_t c, uint8_t wait, int dots) {
  int offset = LEDS / dots;  // pixel separation
  uint32_t color = Wheel(c & 255);

  for (int q=0; q < offset; q++) {
    for (int i=0; i < strip.numPixels(); i=i+offset) {
      strip.setPixelColor(i+q, color);    //turn every third pixel on
    }

    strip.show();
    delay(wait);

    for (int i=0; i < strip.numPixels(); i=i+offset) {
      strip.setPixelColor(i+q, 0);        //turn every n-offset pixel off
    }
  
   }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

