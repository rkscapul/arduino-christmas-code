// Include libraries
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>

// Definitions
#define CONFIG_COMMAND          "0000"
#define WS2812_COMMAND          "1000"
#define PCA9685_COMMAND         "2000"
#define RELAY_COMMAND           "3000"

#define PCA_VALUE_INCREASING    1
#define PCA_VALUE_EQUAL         0
#define PCA_VALUE_DECREASING   -1

#define LANTERN_PIN             3
#define LANTERN_LED_COUNT       60

#define STRIP2_PIN              4
#define STRIP2_LED_COUNT        60

#define WIFI_CONNECTION_LED_PIN 13
#define MDNS_STATUS_LED_PIN     12
#define API_GATEWAY_LED_PIN     11
#define SOURCE_SERIAL_LED_PIN   10
#define SOURCE_CLOUD_LED_PIN    9    
#define SOURCE_LOCAL_LED_PIN    8
 
#define SOURCE_CLOUD_CODE       "25683"
#define SOURCE_LOCAL_CODE       "52547"
#define SOURCE_SERIAL_CODE      "77568"

// Library declarations
Adafruit_NeoPixel lantern(LANTERN_LED_COUNT, LANTERN_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(STRIP2_LED_COUNT, STRIP2_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_PWMServoDriver pca     = Adafruit_PWMServoDriver();

// Variable declarations
char      source[]               = SOURCE_LOCAL_CODE;
int       isApiLedOn             = 0;
int       isMdnsLedOn            = 0;
int       isWiFiConnecting       = 0;
int       isWiFiLedOn            = 0;
int       relayPins[]            = { 22, 23, 24, 25 };
long      previousMillis         = 0;
uint32_t  LED_OFF                = lantern.Color(0, 0, 0);

void setup() {
  // Serial port setup
  Serial.begin(250000);
  Serial.setTimeout(10);

  Serial3.begin(115200);

  // PCA9685 setup
  pca.begin();
  pca.setPWMFreq(1600);

  // Lantern
  lantern.begin();         
  lantern.setBrightness(100);

  // Strip2
  strip2.begin();
  strip2.setBrightness(100);

  // Pins
  pinMode(WIFI_CONNECTION_LED_PIN,  OUTPUT);
  pinMode(MDNS_STATUS_LED_PIN,      OUTPUT);
  pinMode(API_GATEWAY_LED_PIN,      OUTPUT);
  pinMode(SOURCE_SERIAL_LED_PIN,    OUTPUT);
  pinMode(SOURCE_CLOUD_LED_PIN,     OUTPUT);
  pinMode(SOURCE_LOCAL_LED_PIN,     OUTPUT);
//
//  for (int index = 0; index < sizeof(relayPins); index++) {
//    pinMode(relayPins[index], OUTPUT);
//    digitalWrite(relayPins[index], 1);
//  }
}


String command          = "";
String serial3Command   = "";
void loop() {
  if (Serial.available()) {
    char _char = Serial.read();

    if (_char == '\n') {
      runCommand(command);
      command = "";
    } else {
      command += _char;
    }
  }

  while (Serial3.available()) {
    char _char = Serial3.read();
    Serial.write(_char);

    if (_char == '\n') {
      runCommand(serial3Command);
      serial3Command = "";
    } else {
      serial3Command += _char;
    }
  }

  if ((millis() - previousMillis) == 500) {
    previousMillis = millis();
    
    if (isWiFiConnecting) {
      isWiFiLedOn = !isWiFiLedOn;
    }

    digitalWrite(13, isWiFiLedOn);
    digitalWrite(12, isMdnsLedOn);
    digitalWrite(11, isApiLedOn);
  
    digitalWrite(10, strcmp(source, SOURCE_CLOUD_CODE) == 0);
    digitalWrite(9,  strcmp(source, SOURCE_LOCAL_CODE) == 0);
    digitalWrite(8,  strcmp(source, SOURCE_SERIAL_CODE) == 0);

//    Serial.print(isWiFiLedOn);
//    Serial.print(" ");
//    Serial.print(isMdnsLedOn);
//    Serial.print(" ");
//    Serial.print(isApiLedOn);
//    Serial.println(" ");
  }
}

void runCommand(String _command) {
  String mainCommand = _command.substring(0, _command.indexOf(" "));
  
  if (mainCommand == CONFIG_COMMAND) {
    runConfigFunctions(_command);
  } else if (mainCommand == WS2812_COMMAND) {
    runWs2812Functions(_command);
  } else if (mainCommand == PCA9685_COMMAND) {
    runPca9685Functions(_command);
  } else if (mainCommand == RELAY_COMMAND) {
    runRelayFunctions(_command);
  }
}

  void runConfigFunctions(String _command) {
    String functionCode = _command.substring(_command.indexOf(" ") + 1, _command.indexOf("F"));

    if (functionCode == "00") {
      resetLantern();
      resetStrip2();
      resetPca9685();
      resetRelayChannels();
    } else if (functionCode == "05") {
      runSetState(_command);
    } else if (functionCode == "11") {
      runChangeSource(_command);
    }
  }

    void runSetState(String _command) {
      String stateCode = _command.substring(_command.indexOf("F") + 2, _command.indexOf("C"));

      if (stateCode == "01") {
        setWifiState(_command);
      } else if (stateCode == "02") {
        setMdnsState(_command);
      } else if (stateCode == "03") {
        setApiGatewayState(_command);
      }
    }

      void setWifiState(String _command) {
        String value = _command.substring(_command.indexOf("C") + 2, _command.indexOf("B"));

        isWiFiConnecting = value == "5";
        isWiFiLedOn = value == "1";
      }

      void setMdnsState(String _command) {
        String value = _command.substring(_command.indexOf("C") + 2, _command.indexOf("B"));

        isMdnsLedOn = value == "1";
      }

      void setApiGatewayState(String _command) {
        String value = _command.substring(_command.indexOf("C") + 2, _command.indexOf("B"));

        isApiLedOn = value == "1";
      }

    void runChangeSource(String _command) {
      String sourceCode = _command.substring(_command.indexOf("F") + 2, _command.indexOf("S"));

      sourceCode.toCharArray(source, 6);
    }
  



  void runWs2812Functions(String _command) {
    String functionCode = _command.substring(_command.indexOf(" ") + 1, _command.indexOf("F"));

    if (functionCode == "00") {
      resetLantern();
      resetStrip2();
    } else if (functionCode == "10") {
      resetLantern();
    } else if (functionCode == "20") {
      resetStrip2();
    } else if (functionCode == "12") {      
      setLanternPixelColor(_command);
    } else if (functionCode == "22") {
      setStrip2PixelColor(_command);
    }
  }

    void resetLantern() {
      for (int ledIndex = 0; ledIndex < 60; ledIndex++) {
        lantern.setPixelColor(ledIndex, LED_OFF);
      }

      lantern.show();
    }

    void setLanternPixelColor(String _command) {
      int ledIndex = _command.substring(_command.indexOf("F") + 2, _command.indexOf("X")).toInt();
      int red      = _command.substring(_command.indexOf("X") + 2, _command.indexOf("R")).toInt();
      int green    = _command.substring(_command.indexOf("R") + 2, _command.indexOf("G")).toInt();
      int blue     = _command.substring(_command.indexOf("G") + 2, _command.indexOf("B")).toInt();

      lantern.setPixelColor(ledIndex, lantern.Color(red, green, blue));
      lantern.show();
    }

    void resetStrip2() {
      for (int ledIndex = 0; ledIndex < 60; ledIndex++) {
        strip2.setPixelColor(ledIndex, LED_OFF);
      }

      strip2.show();
    }

    void setStrip2PixelColor(String _command) {
      int ledIndex = _command.substring(_command.indexOf("F") + 2, _command.indexOf("X")).toInt();
      int red      = _command.substring(_command.indexOf("X") + 2, _command.indexOf("R")).toInt();
      int green    = _command.substring(_command.indexOf("R") + 2, _command.indexOf("G")).toInt();
      int blue     = _command.substring(_command.indexOf("G") + 2, _command.indexOf("B")).toInt();

      strip2.setPixelColor(ledIndex, strip2.Color(red, green, blue));
      strip2.show();
    }


  void runPca9685Functions(String _command) {
    String functionCode = _command.substring(_command.indexOf(" ") + 1, _command.indexOf("F"));

    if (functionCode == "00") {
      resetPca9685();
    } else if (functionCode == "12") {      
      setPca9685Channel(_command);
    }
  }

    void resetPca9685() {
      for (int index = 0; index < 16; index++) {
        pca.setPWM(index, 0, 4096);
      }
    }

    void setPca9685Channel(String _command) {
      int index   = _command.substring(_command.indexOf("F") + 2, _command.indexOf("X")).toInt();
      int opacity = _command.substring(_command.indexOf("X") + 2, _command.indexOf("O")).toInt();
      
      pca.setPWM(index, 0, (opacity * 16) % 4096);
    }

  void runRelayFunctions(String _command) {
    String functionCode = _command.substring(_command.indexOf(" ") + 1, _command.indexOf("F"));
    
    if (functionCode == "00") {
      resetRelayChannels();
    } else if (functionCode == "12") {      
      setRelayChannelState(_command);
    }
  }

    void resetRelayChannels() {
      for (int index = 0; index < sizeof(relayPins); index++) {
        digitalWrite(relayPins[index], 1);
      }
    }

    void setRelayChannelState(String _command) {
      int index   = _command.substring(_command.indexOf("F") + 2, _command.indexOf("X")).toInt();
      int state   = _command.substring(_command.indexOf("X") + 2, _command.indexOf("B")).toInt();

      digitalWrite(relayPins[index], !state);
    }
