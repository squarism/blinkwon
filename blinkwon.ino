// Hey!
// Particle IDE put these include lines in but you still have to to do the "Add to App" thing.
#include "SparkJson/SparkJson.h"
#include "neopixel/neopixel.h"


// ---- CHANGE THIS ----
#define PIN 6
#define LEDS 24


#define PIXEL_TYPE WS2812B
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, PIXEL_TYPE);

// status light colors
int colorStart = 0;
int colorEnd = 255;
int colorRotationSlowness = defaultColorRotationSpeed();

char *theme = "unknown";


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(defaultBrightness());
  
  Particle.subscribe("squarism/blinkwon", eventHandler);
  setColor(theme);
}

void connect() {
  if (Particle.connected() == false) {
    Particle.connect();
  }
}

void loop() {
  connect();
  Particle.process();

  if (strcmp(theme, "unknown") == 0 ) {
    strip.setBrightness(50);
    dotChase(245, 35, 2);
  } else {
    colorRing(colorRotationSlowness);
  }
}

int defaultColorRotationSpeed() {
   return 200; 
}

int defaultBrightness() {
    return 210;
}

void eventHandler(const char *event, const char *data) {
    int len = strlen(data);
    char dataCopy[len+1];
    strcpy(dataCopy, data);


    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(dataCopy);
    
    if (json.success() && json.containsKey("theme"))
    {
        theme = const_cast<char*>(json["theme"].asString());
        setColor(theme);
    } 
    
    if (json.success() && json.containsKey("idle"))
    {
        theme = "unknown";
    }
    
}

void setColor(const char *theme) {
    colorRotationSlowness = defaultColorRotationSpeed();
    strip.setBrightness(defaultBrightness());
    
    // wow, you can't easily do switch statements in C?
    if (strcmp(theme, "green") == 0) {
        colorStart = 5;
        colorEnd = 20;
    } else if (strcmp(theme, "red") == 0) {
        colorStart = 80;
        colorEnd = 95;
    } else if (strcmp(theme, "blacklight") == 0 ) {
        colorStart = 135;
        colorEnd = 160;
    } else if (strcmp(theme, "yellow") == 0 ) {
        colorStart = 50;
        colorEnd = 70;
    } else if (strcmp(theme, "blue") == 0 ) {
        colorStart = 160;
        colorEnd = 180;
    } else if (strcmp(theme, "bluegreen") == 0 ) {
        colorStart = 190;
        colorEnd = 210;
    } else if (strcmp(theme, "orange") == 0 ) {
        colorStart = 60;
        colorEnd = 80;
    } else if (strcmp(theme, "pink") == 0 ) {
        colorStart = 90;
        colorEnd = 110;
    } else if (strcmp(theme, "rainbow") == 0 ) {
        colorStart = 0;
        colorEnd = 255;
    } else if (strcmp(theme, "off") == 0 ) {
        strip.setBrightness(0);
        colorStart = 1;
        colorStart = 2;
    } else {
        strip.setBrightness(5);
    }
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

