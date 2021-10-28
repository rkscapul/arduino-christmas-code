// Definitions 

#define SR_DATA_PIN 6
#define SR_LATCH_PIN 4 
#define SR_CLOCK_PIN 7

// Declarations - General

String command;
int leds = 0;
long previousMillis = 0; 

// Declarations - Indicators

bool stripTestMode = false;
bool stripTestModeBlink = false;

bool pcaTestMode = false;
bool pcaTestModeBlink = false;

bool staticTestMode = false;
bool staticTestModeBlink = false;

bool computerControlMode = true;
bool arduinoControlMode = false;

void setup() {
  pinMode(SR_DATA_PIN, OUTPUT);
  pinMode(SR_LATCH_PIN, OUTPUT);
  pinMode(SR_CLOCK_PIN, OUTPUT);

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

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > 500) {
    
    previousMillis = currentMillis;   

    setIndicatorLeds();
  }
  
}


void parseCommand(String com) {
  String _cmd = com.substring(0, com.indexOf(" "));
  
  Serial.print("command code: ");
  Serial.println(_cmd);

  if (_cmd == "10910") {
    
    computerControlMode = true;
    arduinoControlMode = false;
    
  } else if (_cmd == "10273") {
    
    computerControlMode = false;
    arduinoControlMode = true;
    
  } else if (_cmd == "10505") {
    
    stripTestMode = false;
    pcaTestMode = false;
    staticTestMode = false;
    
  } else if (_cmd == "20505") {
    
    stripTestMode = !stripTestMode;
    
  } else if (_cmd == "21505") {
    
    pcaTestMode = !pcaTestMode;
    
  } else if (_cmd == "22505") {
    
    staticTestMode = !staticTestMode;
    
  }
}

void setIndicatorLeds() {
  int value = 0;

  value += !stripTestMode ? 1 : 0;
  value += stripTestMode && stripTestModeBlink ? 2 : 0;

  value += !pcaTestMode ? 4 : 0;
  value += pcaTestMode && pcaTestModeBlink ? 8 : 0;

  value += !staticTestMode ? 16 : 0;
  value += staticTestMode && staticTestModeBlink ? 32 : 0;

  value += computerControlMode ? 64 : 0;
  value += arduinoControlMode ? 128 : 0;

  updateShiftRegisters(value);

  stripTestModeBlink = !stripTestModeBlink;
  pcaTestModeBlink = !pcaTestModeBlink;
  staticTestModeBlink = !staticTestModeBlink;
}

void updateShiftRegisters(int code) {
  
  digitalWrite(SR_LATCH_PIN, LOW);
  shiftOut(SR_DATA_PIN, SR_CLOCK_PIN, LSBFIRST, code);
  digitalWrite(SR_LATCH_PIN, HIGH);
}
