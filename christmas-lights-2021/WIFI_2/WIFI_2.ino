#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Definitions
#define SRL_RX D6       // Serial RX pin
#define SRL_TX D5       // Serial TX pin

#define SVR_PORT 80     // Web server port

#define WIFI_DELAY 500  // WiFi retry delay
#define WIFI_RETRY 50   // WiFi max initial retry

// Structs
struct MessagePayload {
  char* message;
};

struct TestModePayload {
  bool ledStrip;
  bool pcaArray;
};

// Declarations
const char* WIFI_SSID = "PLDTHOMEFIBR98300";
const char* WIFI_PWRD = "PLDTWIFIt9h6e";

char* command = "";

TestModePayload testMode;

ESP8266WebServer webServer(SVR_PORT);
SoftwareSerial SrlComm(SRL_RX, SRL_TX);

void setup() {

  // Setup variable values
  setupVariables();
  
  Serial.begin(9600);
  
  SrlComm.begin(9600);

  Serial.print("Connecting to '");
  Serial.print(WIFI_SSID);
  Serial.print("' ");
  
  WiFi.begin(WIFI_SSID, WIFI_PWRD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected. Your IP address is ");
  Serial.println(WiFi.localIP());

  setupRestEndpoints();

  webServer.begin();
  Serial.println("HTTP REST Server started.");
}

void sendMessageResponse(char* message) {
  StaticJsonDocument<200> doc;
  char payload[200];

  doc["message"] = message;

  serializeJson(doc, payload, sizeof(doc));
  webServer.send(200, "application/json", payload);
}

void setupVariables() {
  testMode.ledStrip = false;
  testMode.pcaArray = false;
}


// Endpoint functions
void setTestMode() { 
  bool anyChanges = false;
  DynamicJsonDocument doc(1024);
  
  if (webServer.hasArg("plain") == false){
    sendMessageResponse("Body not received.");
    return;
  }

  // Read request payload.
  String request = webServer.arg("plain");

  // Deserialize and convert request to JSON object.
  deserializeJson(doc, request);
  JsonObject payload_ = doc.as<JsonObject>();

  // Print new values to serial monitor, 
  // and store new values to existing variable if changed.
  Serial.println("Change of configuration setting was requested.");
  
  if (testMode.ledStrip != payload_["ledStrip"]) {
    testMode.ledStrip = payload_["ledStrip"];

    Serial.print("LED Strip Test Mode: ");
    Serial.println(testMode.ledStrip);

    anyChanges = true;

    sendCommand("0000 121F 10S");
  }

  if (testMode.pcaArray != payload_["pcaArray"]) {
    testMode.pcaArray = payload_["pcaArray"];
  
    Serial.print("PCA Array Test Mode: ");
    Serial.println(testMode.pcaArray);

    anyChanges = true;
    
    sendCommand("0000 121F 20S");
  }

  if (anyChanges) {
    sendMessageResponse("Payload received. Updating configurations.");
  } else {
    sendMessageResponse("Payload received. No changes in configuration.");
  }
}


// Setup REST Endpoints
void setupRestEndpoints() {
  // {{ip_address:port}}/
  webServer.on("/", HTTP_GET, []() {
    sendMessageResponse("Hello, I'm active right now");
  });

  // {{ip_address:port}}/config
  

  webServer.on("/config/test-mode", HTTP_POST, setTestMode);
}


// Command management
void sendCommand(char* _command) {
  command = _command;
}

void loop() {
  webServer.handleClient();
  
  if (command != "") {
    Serial.println(command);
    SrlComm.write(command);
    SrlComm.write("\n");
  }

  if (SrlComm.available() > 0 && command != "")
  {
    Serial.println("Command sent.");
    command = "";
  }
}
