// This #include statement was automatically added by the Particle IDE.
// #include <SparkJson.h>

// This little interface is supposed to replace my crappy SSH and cronjob
// workflow.  I want to hit a button and switch theme.  Movie mode
// will set a cloud variable that any client would check for first
// before setting the theme, thus disabling the auto-schedule.


// Quick Theme Switching Buttons
#define BUTTON1_PIN D0
#define BUTTON2_PIN D1
#define BUTTON3_PIN D2

// Movie Time Mode Switch
#define SWITCH1_PIN D3

int button1_value;
int button2_value;
int button3_value;

int switch1_value = LOW;
int switch1_previous_value = LOW;

char theme[15];


void debug(String message) {
    char msg [50];
    sprintf(msg, message.c_str());
    Spark.publish("DEBUG", msg, 60, PRIVATE);
}

void setup() {
    pinMode(BUTTON1_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON2_PIN, INPUT_PULLDOWN);
    pinMode(BUTTON3_PIN, INPUT_PULLDOWN);
    pinMode(SWITCH1_PIN, INPUT_PULLDOWN);
    
    Particle.variable("movietime", &switch1_value, INT);
}

bool isButtonPressed(int pin) {
    return (digitalRead(pin) == HIGH);
}

bool hasSwitchChanged(int pin) {
    // oh sweet side effects dude
    switch1_value = digitalRead(SWITCH1_PIN);
    
    if (switch1_value == !switch1_previous_value) {
        switch1_previous_value = switch1_value;
        return true;
    }
    return false;
}

void sendThemeChange(const char *channel, const char *theme, int brightness) {
    char jsonBuffer[60];
    sprintf(jsonBuffer, "{\"theme\":\"%s\", \"brightness\":%d}", theme, brightness);
    Spark.publish(channel, jsonBuffer, PRIVATE);
}

void sendBrightnessChange(const char *channel, int brightness) {
    char jsonBuffer[60];
    sprintf(jsonBuffer, "{\"brightness\":%d}", brightness);
    Spark.publish(channel, jsonBuffer, PRIVATE);
}

void loop() {
    if (isButtonPressed(BUTTON1_PIN)) {
        // debug("BUTTON 1 pressed.");
        // main
        sendThemeChange("squarism/blinkwon", "white", 60);
        // accent strips
        sendThemeChange("squarism/ambient_strip", "white", 80);
        delay(1000);
    }
    
    if (isButtonPressed(BUTTON2_PIN)) {
        // debug("BUTTON 2 pressed.");
        sendThemeChange("squarism/blinkwon", "off", 0);
        sendThemeChange("squarism/ambient_strip", "off", 0);
        delay(1000);
    }

    if (isButtonPressed(BUTTON3_PIN)) {
        // debug("BUTTON 3 pressed.");
        sendThemeChange("squarism/blinkwon", "bluegreen", 20);
        sendThemeChange("squarism/ambient_strip", "red", 20);
        delay(1000);
    }

    if (hasSwitchChanged(SWITCH1_PIN)) {
        // debug("SWITCH 1 changed.");
        if (switch1_value == HIGH) {
            // movie time
        } else {
            // normal time
        }
        delay(1000);
    }
    
    delay(15);  // an eternity
}
