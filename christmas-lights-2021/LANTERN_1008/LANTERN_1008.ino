// Include libraries
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>
#include <FastLED.h>

// Definitions
#define PCA_VALUE_INCREASING    1
#define PCA_VALUE_EQUAL         0
#define PCA_VALUE_DECREASING   -1

#define WS_PIN                  3
#define WS_LED_COUNT            60

// Declarations
Adafruit_NeoPixel strip(WS_LED_COUNT, WS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

CRGB leds[WS_LED_COUNT];


int index = 0;

int pcaPinCurrentValue[16];
int pcaPinIncrementValue[16];
int pcaPinLockedValue[16];
int pcaPinLockedValueState[16];

int channels[196];

int rgb = 0;

void setup() {
  // Serial port setup
  Serial.begin(115200);

  // PCA9685 setup
  pca.begin();
  pca.setPWMFreq(1600);

//  resetPcaPinValues();

  strip.begin();         
  strip.setBrightness(100);
//
//  pcaPinIncrementValue[0] = 128;
//  pcaPinIncrementValue[1] = 64;
//
//  for (int x = 0; x < 196; x++) {
//     channels[x] = 0;
//  }
//
//  FastLED.addLeds<WS2812, WS_PIN, GRB>(leds, WS_LED_COUNT);
//  FastLED.setBrightness( 100 );
//  FastLED.show();
}

void loop() {
//  for (int index = 0; index < 16; index++) {
//    if (pcaPinLockedValueState[index] == PCA_VALUE_INCREASING) {
//      pcaPinCurrentValue[index] += pcaPinIncrementValue[index];
//    } else if (pcaPinLockedValueState[index] == PCA_VALUE_DECREASING) {
//      pcaPinCurrentValue[index] -= pcaPinIncrementValue[index];
//    }
//
//    if (pcaPinCurrentValue[index] == pcaPinLockedValue[index]) {
//      pcaPinLockedValueState[index] = PCA_VALUE_EQUAL;
//    }
//
//    pca.setPWM(index, 0, pcaPinCurrentValue[index] % 4096);
//  }
//
//  if (pcaPinCurrentValue[0] == pcaPinLockedValue[0]) {
//    pcaPinLockedValue[0] = pcaPinLockedValue[0] == 3968 ? 0 : 3968;
//    if (pcaPinCurrentValue[0] < pcaPinLockedValue[0]) {
//      pcaPinLockedValueState[0] = PCA_VALUE_INCREASING;
//    } else {
//      pcaPinLockedValueState[0] = PCA_VALUE_DECREASING;
//    }
//  }
//
//  if (pcaPinCurrentValue[1] == pcaPinLockedValue[1]) {
//    pcaPinLockedValue[1] = pcaPinLockedValue[1] == 3968 ? 0 : 3968;
//    if (pcaPinCurrentValue[1] < pcaPinLockedValue[1]) {
//      pcaPinLockedValueState[1] = PCA_VALUE_INCREASING;
//    } else {
//      pcaPinLockedValueState[1] = PCA_VALUE_DECREASING;
//    }
//  }

    




    // Turn the LED on, then pause
//  leds[0] = CRGB::Red;
//  FastLED.show();
//  FastLED.delay(500);
//  // Now turn the LED off, then pause
//  leds[0] = CRGB::Black;
//  FastLED.show();
//  FastLED.delay(500);
//  Serial.print(index);
//  Serial.print(" ");
//
//  
//
////  while(!Serial.available());
//    channels[index] = index;
//
//  index++;
//
//  if (index >= 16) {
////    while(!Serial.available());
//      channels[index] = rgb;
//
//    index++;
//      
////    while(!Serial.available());
//      channels[index] = rgb;
//
//    index++;
//
//    leds[index] = CRGB(channels[index - 2], channels[index - 1], channels[index]);
//    FastLED.show();
//
//    Serial.print(channels[index - 2]);
//    Serial.print(" ");
//    Serial.print(channels[index - 1]);
//    Serial.print(" ");
//    Serial.print(channels[index]);
//  
//    rgb++;
//    if (rgb >= 255) rgb = 0;
//  }
//
////  index++;
////  
////  FastLED.show();
//  if (index >= 196) index = 0;
//  Serial.println();

//  Serial.println(Serial.available());

    for (int x = 16; x < 196; x++) {
      while(!Serial.available())
        channels[x] = Serial.read();
    }
  
//  for (int x = 16; x < 196; x++) {
//    while (Serial.available() > 0);
//      storeValue(x, Serial.read());
//  }
////    if (x < 16) {
////      storeValue(x, x);
////    } else {
////      storeValue(x, rgb);
////      storeValue(x, rgb);
////      storeValue(x, rgb);
////  
//////      leds[x] = CRGB(rgb, rgb, rgb);
//////      FastLED.show();
////      
////      rgb++;
//////      if (rgb >= 255) rgb = 0;
////    }
//  }
//
//  
//
//  delay(1000);
//

//  if (index == 196) {


//  for (int x = 0; x < 16; x++) {
//    pca.setPWM(index, 0, pcaPinCurrentValue[index] % 4096);
//  }

  int ledI = 0;
  for (int x = 16; x < 194; x += 3) {
//     Serial.print(channels[x]);
//     Serial.print(" -> ");
//     Serial.print(channels[x + 1]);
//     Serial.print(" -> ");
//     Serial.println(channels[x + 2]);
    strip.setPixelColor(ledI, strip.Color(channels[x], channels[x + 1], channels[x + 2]));
    strip.show();
    

    ledI++;
  }

//  Serial.println(rgb);
//delay(50);
//FastLED.show();

  rgb += 25;
  if (rgb >= 250) rgb = 0;
//  }
}

void storeValue(int index, int value) {
  channels[index] = value;

//  Serial.print(index);
//  Serial.print(" -> ");
////  Serial.print(value);
////  Serial.print(" -> ");
//  Serial.println(channels[index]);
  
//  index++;
}

void updatePcaPins() {
  for (int index = 0; index < 196; index++) {
    
  }
}

void resetPcaPinValues() {
  for (int index = 0; index < 16; index++) {
    pcaPinCurrentValue[index] = 0;
    pcaPinLockedValue[index] = 0;
    pca.setPWM(index, 0, 4096);
    pcaPinLockedValueState[index] = 0;
    pcaPinIncrementValue[index] = 0;
  }
}
