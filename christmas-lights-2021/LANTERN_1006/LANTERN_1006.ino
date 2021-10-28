// Include libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Definitions
#define PCA_VALUE_INCREASING    1
#define PCA_VALUE_EQUAL         0
#define PCA_VALUE_DECREASING   -1


// Declarations
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver();

int pcaPinCurrentValue[16];
int pcaPinIncrementValue[16];
int pcaPinLockedValue[16];
int pcaPinLockedValueState[16];

void setup() {
  // Serial port setup
  Serial.begin(9600);

  // PCA9685 setup
  pca.begin();
  pca.setPWMFreq(1600);

  resetPcaPinValues();

  pcaPinIncrementValue[0] = 128;
  pcaPinIncrementValue[1] = 64;
}

void loop() {
  for (int index = 0; index < 16; index++) {
    if (pcaPinLockedValueState[index] == PCA_VALUE_INCREASING) {
      pcaPinCurrentValue[index] += pcaPinIncrementValue[index];
    } else if (pcaPinLockedValueState[index] == PCA_VALUE_DECREASING) {
      pcaPinCurrentValue[index] -= pcaPinIncrementValue[index];
    }

    if (pcaPinCurrentValue[index] == pcaPinLockedValue[index]) {
      pcaPinLockedValueState[index] = PCA_VALUE_EQUAL;
    }

    pca.setPWM(index, 0, pcaPinCurrentValue[index] % 4096);
  }

  if (pcaPinCurrentValue[0] == pcaPinLockedValue[0]) {
    pcaPinLockedValue[0] = pcaPinLockedValue[0] == 3968 ? 0 : 3968;
    if (pcaPinCurrentValue[0] < pcaPinLockedValue[0]) {
      pcaPinLockedValueState[0] = PCA_VALUE_INCREASING;
    } else {
      pcaPinLockedValueState[0] = PCA_VALUE_DECREASING;
    }
  }

  if (pcaPinCurrentValue[1] == pcaPinLockedValue[1]) {
    pcaPinLockedValue[1] = pcaPinLockedValue[1] == 3968 ? 0 : 3968;
    if (pcaPinCurrentValue[1] < pcaPinLockedValue[1]) {
      pcaPinLockedValueState[1] = PCA_VALUE_INCREASING;
    } else {
      pcaPinLockedValueState[1] = PCA_VALUE_DECREASING;
    }
  }
}

void updatePcaPins() {
  for (int index = 0; index < 16; index++) {
    
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
