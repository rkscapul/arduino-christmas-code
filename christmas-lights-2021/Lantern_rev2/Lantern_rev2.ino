#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Definitions - Shift Register Indicators
#define SRI_DATA_PIN 2
#define SRI_LATCH_PIN 22
#define SRI_CLOCK_PIN 23







String payload;

// Declarations - Shift Register Indicators
bool sriBlink = false;
long sriPreviousMillis = 0;
long pcaPreviousMillis = 0;

// Declarations - PCA9685 Modules
Adafruit_PWMServoDriver pca[] = { 
  Adafruit_PWMServoDriver(0x40),
  Adafruit_PWMServoDriver(0x41),
  Adafruit_PWMServoDriver(0x42)
};

int pcaCount = 3;
int pcaControllerIndex = 0;
int pcaLedIndex = 0;

bool pcaAllLedsOn = false;

// Declarations - Configurations
String configSource     = "33348";
bool configPcaTestMode  = true;
bool configWsTestMode   = false;

void setup()
{
  // Shift register pin config
  pinMode(SRI_DATA_PIN, OUTPUT);
  pinMode(SRI_LATCH_PIN, OUTPUT);
  pinMode(SRI_CLOCK_PIN, OUTPUT);
  
  Serial.begin(9600);

  // PCA9685 configuration
  //pwmOne.begin();
  //pwmOne.setPWMFreq(1600);
  setupPca9685();
  //pca1.begin();
  //pca1.setPWMFreq(1600);

//  pca[0].begin();
//  pca[0].setPWMFreq(1600);
}

void setupPca9685() {
  for (int x = 0; x < pcaCount; x++) {
    pca[x].begin();
    pca[x].setPWMFreq(1600);
  }
}


void loop()
{
  int data=50;
  while(Serial.available())
  {
    char c = Serial.read();
    
    if (c == '\n') {
      Serial.println(payload);
      parseCommand(payload);
      payload = "";
    } else {
      payload += c;
    }

    
  }

  unsigned long sriCurrentMillis = millis();
  if(sriCurrentMillis - sriPreviousMillis > 500) {
    
    sriPreviousMillis = sriCurrentMillis;   

    setIndicatorLeds();
  }

  if (configPcaTestMode) {
    unsigned long pcaCurrentMillis = millis();
    if(pcaCurrentMillis - pcaPreviousMillis > 500) {
      
      pcaPreviousMillis = pcaCurrentMillis;   
  
      testModePca();
    }
  }
}

void updateShiftRegisters(int code) {
  digitalWrite(SRI_LATCH_PIN, LOW);
  shiftOut(SRI_DATA_PIN, SRI_CLOCK_PIN, LSBFIRST, code);
  digitalWrite(SRI_LATCH_PIN, HIGH);
}

void parseCommand(String command) {
  String cmd = command.substring(0, command.indexOf(" "));

  if (cmd == "0000") {

    // Configuration commands.
    parseConfigurationFunctions(command);
    
  } else if (cmd == "1000") {

    // PCA9685 commands.
    parsePca9685Commands(command);
    
  }
}





// Configuration commands. //
// Indicator LEDs
// Test mode state

void parseConfigurationFunctions(String command) {
  String code = command.substring(command.indexOf(" ") + 1, command.indexOf("F"));

  if (code == "111") {

    // Set source
    // 33348 - 
    // 27366 - Arduino Communication
    // 94610 - Windows Serial Port
    parseFunction_111(command);
    
  } else if (code == "121") {

    // Set Test Mode
    parseFunction_121(command);
    
  }
}

void parseFunction_111(String command) {
  // Set source
  String source = command.substring(command.indexOf("F") + 2, command.indexOf("S"));

  configSource = source;
}

void parseFunction_121(String command) {
  String source = command.substring(command.indexOf("F") + 2, command.indexOf("S"));
  
  // Set test mode
  if (source == "10") {
    configPcaTestMode = !configPcaTestMode;
    resetPcaLeds();
  } else if (source == "20") {
    configWsTestMode = !configWsTestMode;
  } else if (source == "30") {
    
  }
  
}

void setIndicatorLeds() {
  // Default Power
  int value = 1;

  // State
  value += 2;

  // Source
  value += configSource == "33348" ? 4  : 0; // Windows Serial Port
  value += configSource == "27366" ? 8  : 0; // Arduino Communication
  value += configSource == "94610" ? 16 : 0; // Windows Serial Port

  // Output status (Blinking = Test mode; Steady = Normal)
  value += !configPcaTestMode ? 32 : 0;
  value += configPcaTestMode && sriBlink ? 32 : 0;
  
  value += !configWsTestMode ? 64 : 0;
  value += configWsTestMode && sriBlink ? 64 : 0;
  value += configWsTestMode && sriBlink ? 128 : 0;
  
  updateShiftRegisters(value);

  sriBlink = !sriBlink;
}



// PCA9685 Commands //
void parsePca9685Commands(String command) {
  String code = command.substring(command.indexOf(" ") + 1, command.indexOf("F"));

  if (code == "100") {

    // Reset all PCA leds
    resetPcaLeds();
    
  } else if (code == "111") {

    // Turn on PCA led
    turnOnPcaLed(command);
    
  } else if (code == "121") {

    // Turn on PCA led
    turnOffPcaLed(command);
    
  }
}

void testModePca() {
  if (pcaAllLedsOn == false) {
    pca[pcaControllerIndex].setPWM(pcaLedIndex, 4096, 0);
  } else {
    pca[pcaControllerIndex].setPWM(pcaLedIndex, 0, 4096);
  }

  pcaLedIndex++;
  if (pcaLedIndex > 15) {
    pcaControllerIndex++;
    pcaLedIndex = 0;
  }

  if (pcaControllerIndex == pcaCount) {
    pcaControllerIndex = 0;
    pcaAllLedsOn = !pcaAllLedsOn;
  }
}

int getPcaControllerIndex(String command) {
  return command.substring(command.indexOf("F") + 2, command.indexOf("C")).toInt();
}

int getPcaPinIndex(String command) {
  return command.substring(command.indexOf("C") + 2, command.indexOf("X")).toInt();
}

void resetPcaLeds() {
  for (int controller = 0; controller < pcaCount; controller++) {
    for (int index = 0; index < 16; index++) {
      pca[controller].setPWM(index, 0, 4096);
    }
  }

  pcaControllerIndex = 0;
  pcaLedIndex = 0;
}

void turnOnPcaLed(String command) {
  int pcaController = getPcaControllerIndex(command);
  int pcaPin = getPcaPinIndex(command);

  pca[pcaController].setPWM(pcaPin, 4096, 0);
}

void turnOffPcaLed(String command) {
  int pcaController = getPcaControllerIndex(command);
  int pcaPin = getPcaPinIndex(command);

  pca[pcaController].setPWM(pcaPin, 0, 4096);
}
