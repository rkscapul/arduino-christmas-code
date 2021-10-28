#include <Adafruit_PWMServoDriver.h>
#define PCA_PINS 16

Adafruit_PWMServoDriver pwmOne = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwmTwo = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwmThree = Adafruit_PWMServoDriver(0x42);

String command;
int testModeIndex = 0;
bool testModeAllPinsOn = false;
long previousMillis = 0; 
bool testMode = true;

void setup() {
  // PWM configuration
  pwmOne.begin();
  pwmOne.setPWMFreq(1600);

  pwmTwo.begin();
  pwmTwo.setPWMFreq(1600);

  pwmThree.begin();
  pwmThree.setPWMFreq(1600);

  // Serial port configurartion
  Serial.begin(9600);
  Serial.setTimeout(10);
}

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

  if (testMode == true) {
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > 25) {
      
      previousMillis = currentMillis;   

      if (testModeAllPinsOn == false) {
        pwmOne.setPWM(testModeIndex, 4096, 0);
        pwmTwo.setPWM((PCA_PINS - (testModeIndex + 1)), 4096, 0);
        pwmThree.setPWM(testModeIndex, 4096, 0);
      } else {
        pwmOne.setPWM(testModeIndex, 0, 4096);
        pwmTwo.setPWM((PCA_PINS - (testModeIndex + 1)), 0, 4096);
        pwmThree.setPWM((PCA_PINS - (testModeIndex + 1)), 0, 4096);
      }

      testModeIndex++;
      if (testModeIndex >= PCA_PINS) {
        testModeIndex = 0;
        testModeAllPinsOn = !testModeAllPinsOn;
      }
    }
  }
}

int getPcaPinIndex(String com) {
  return com.substring(com.indexOf(" ") + 1, com.indexOf("X")).toInt();
}

void parseCommand(String com) {
  String _cmd = com.substring(0, com.indexOf(" "));
  
  Serial.print("command code: ");
  Serial.print(_cmd);

  if (_cmd == "20505") {
    _testMode();
  } else if (_cmd == "20000" && testMode == false) {
    resetAllPcaLeds();
  } else if (_cmd == "21001" && testMode == false) {
    turnOnPcaLed(com);
  } else if (_cmd == "21000" && testMode == false) {
    turnOffPcaLed(com);
  } else if (_cmd == "21011" && testMode == false) {
    // fadeIn(com);
  } else if (_cmd == "21010" && testMode == false) {
    // fadeOut(com);
  } 
}

void resetAllPcaLeds() {
  for (int x = 0; x < PCA_PINS; x++) {
    pwmOne.setPWM(x, 0, 4096);
    pwmTwo.setPWM(x, 0, 4096);
  }
}

//void fadeIn(String com) {
//  int _index = getPcaPinIndex(com);
//  
//  for (int i = 0; i < 4096; i += 8) {
//      pwm.setPWM(_index, 0, (i) % 4096 );
//  }
//}
//
//void fadeOut(String com) {
//  int _index = getPcaPinIndex(com);
//  
//  for (int j = 4096; j >= 0; j -= 8) {
//      pwm.setPWM(_index, 0, j);
//  }
//}
//
void turnOnPcaLed(String com) {
  int _index = getPcaPinIndex(com);
  
  pwmOne.setPWM(_index, 4096, 0);
  pwmTwo.setPWM(_index, 4096, 0);
}


void turnOffPcaLed(String com) {
  int _index = getPcaPinIndex(com);
  
  pwmOne.setPWM(_index, 0, 4096);
  pwmTwo.setPWM(_index, 0, 4096);
}

void _testMode() {
  testMode = !testMode;

  Serial.print(" - pca test mode (");
  Serial.print(testMode);
  Serial.println(")");
  
  
  if (testMode == false) {
    testModeIndex = 0;
    testModeAllPinsOn = false;
  }

  resetAllPcaLeds();
}
