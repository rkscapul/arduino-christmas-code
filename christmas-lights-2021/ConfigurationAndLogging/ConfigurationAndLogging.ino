#include <ArduinoJson.h>
#include <SD.h>
#include <SPI.h>

#define SD_CHIP_PIN 4

struct Config {
  int  recieverCode;
  bool testModeLedStrip;
  bool testModePcaArray;
  bool testModeStaticPins;
};

String command;
const char *configFile = "/settings.txt";
Config config_;

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(10);

  pinMode(10, OUTPUT);

  SD.begin(SD_CHIP_PIN);

  loadConfiguration();
  printConfigValues();
}

void loadConfiguration() {
  File file = SD.open(configFile);
  StaticJsonDocument<512> doc;

  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  config_.recieverCode = doc["recieverCode"];
  config_.testModeLedStrip = doc["testModeLedStrip"];
  config_.testModePcaArray = doc["testModePcaArray"];
  config_.testModeStaticPins = doc["testModeStaticPins"];

  file.close();
}

void saveConfiguration() {
  SD.remove(configFile);

  File file = SD.open(configFile, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  StaticJsonDocument<256> doc;

  doc["recieverCode"] = config_.recieverCode;
  doc["testModeLedStrip"] = config_.testModeLedStrip;
  doc["testModePcaArray"] = config_.testModePcaArray;
  doc["testModeStaticPins"] = config_.testModeStaticPins;

  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

void printConfigValues() {
  Serial.println("Values");
  Serial.println(config_.recieverCode);
  Serial.println(config_.testModeLedStrip);
  Serial.println(config_.testModePcaArray);
  Serial.println(config_.testModeStaticPins);
}

void parseCommand(String com) {
  String _cmd = com.substring(0, com.indexOf(" "));
  
  Serial.print("command code: ");
  Serial.println(_cmd);

  if (_cmd == "10808") {
    // Set receiver
    // 10910 - PC serial port
    // 10919 - Arduino communication
    setReceiver(com);
  } else if (_cmd == "10111") { 

    printConfigValues();
    
  } else if (_cmd == "20505") {
    
    config_.testModeLedStrip = !config_.testModeLedStrip;
    
  } else if (_cmd == "21505") {
    
    config_.testModePcaArray = !config_.testModePcaArray;
    
  } else if (_cmd == "22505") {
    
    config_.testModeStaticPins = !config_.testModeStaticPins;
    
  }

  saveConfiguration();
}

void setReceiver(String com) {
  String code = com.substring(com.indexOf(" ") + 1, com.indexOf("R"));

  config_.recieverCode = code.toInt();
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
}
