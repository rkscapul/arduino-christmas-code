// Include libraries
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>

// Definitions
#define NP1_PIN                 3
#define NP1_COUNT               60

#define PCA_VALUE_INCREASING    1
#define PCA_VALUE_EQUAL         0
#define PCA_VALUE_DECREASING   -1

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


// Declarations
Adafruit_NeoPixel strip(NP1_COUNT, NP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

int pcaPinCurrentValue[16];
int pcaPinIncrementValue[16];
int pcaPinLockedValue[16];
int pcaPinLockedValueState[16];

int np1R[60];
int np1G[60];
int np1B[60];



void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  
  // Serial port setup
  Serial.begin(115200);

  // PCA9685 setup
  pca.begin();
  pca.setPWMFreq(1600);

  // NP1
  strip.begin();         
  strip.setBrightness(100);
}

void loop() {

  uint32_t r, g, b;
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
  for(int x = 0; x < 60; x++) {
    while(!Serial.available());
      r = Serial.read();
    while(!Serial.available());
      g = Serial.read();
    while(!Serial.available());
      b = Serial.read();  
    
    strip.setPixelColor(x, strip.Color(r, g, b));
    strip.show();
  }

  
}
