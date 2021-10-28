#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN 6
#define LED_COUNT 60

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();
  strip.show();
  strip.setBrightness(50);

  Serial.begin(9600);
  Serial.setTimeout(10);
}



String command;

void loop() {

  if (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      parseCommand(command);
      command = "";
    } else {
      command += c;
    }
  }
  
}

void parseCommand(String com) {
  String _cmd = com.substring(0, com.indexOf(" "));

  if (_cmd == "0000") {
    resetAllLeds();
  } else if (_cmd == "1001") {
    setLedColor(com);
  }
}

void setLedColor(String com) {
  int _index = com.substring(com.indexOf(" ") + 1, com.indexOf("X")).toInt();
  int _red = com.substring(com.indexOf("X") + 2, com.indexOf("R")).toInt();
  int _green = com.substring(com.indexOf("R") + 2, com.indexOf("G")).toInt();
  int _blue = com.substring(com.indexOf("G") + 2, com.indexOf("B")).toInt();

  strip.setPixelColor(_index, strip.Color(_red, _green, _blue));
  strip.show();
}

void resetAllLeds() {
  for (int x = 0; x < LED_COUNT; x++) {
    strip.setPixelColor(x, strip.Color(0, 0, 0));
  }

  strip.show();
}
