// Import libraries
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Definitions
#ifndef STASSID
#define STASSID             "PLDTHOMEFIBR98300"
#define STAPSK              "PLDTWIFIt9h6e"
#endif

#define CONFIG_COMMAND_CODE "0000"
#define NP_COMMAND_CODE     "1000"
#define PCA_COMMAND_CODE    "2000"

// Library declarations
ESP8266WebServer server(80);

// Variable declaration
StaticJsonDocument<200> body;

const char* ssid     = STASSID;
const char* password = STAPSK;

void setup() {
  Serial.begin(115200);

  // WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("0000 05F 01C 5B");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("0000 05F 01C 1B");

  // MDNS
  if (MDNS.begin("esp8266")) {
    Serial.println("0000 05F 02C 1B");
  }

  // API server
  setupServerRouting();
  server.begin();
  Serial.println("0000 05F 03C 1B");
}

  void setupServerRouting() {
    // Handling error 404
    server.onNotFound(handleError404);

    // Setup GET endpoints
    server.on("/", HTTP_GET, getHelloResponse);

    // Setup POST endpoints
    server.on("/reset-lights", postResetAllLights);
    server.on("/set-source", postSetSource);
  }
  
    void handleError404() {
      server.send(404, "text/json", "{ \"message\": \"Endpoint not found.\" }");
    }

    void getHelloResponse() {
      sendResponse("Hello from Christmas Lights API");
    }

//    void postTurnOnLight() {
//      if (!server.hasArg("plain")) {
//        sendResponse("Body not received");
//        return;
//      }
//
////      String body = server.arg("plain");
////      char bodyArray[body.length()];
////      
////      body.toCharArray(bodyArray, body.length());
////      deserializeJson(document, bodyArray);
////
////      const char* group = document["group"];
//
////      server.send(200, "text/plain", group);
//    }

    void postResetAllLights() {
      if (!server.hasArg("plain")) {
        sendResponse("Body not received");
        return;
      }

      sendResetLightGroupCommand(server.arg("group"));
      sendResponse("Light group have been resetted to off");
    }

      void sendResetLightGroupCommand(String group) {
        if (group == "all") {
          Serial.println("0000 00F");
        } else if (group == "all-ws-groups") {
          Serial.println("1000 00F");
        } else if (group == "lantern") {
          Serial.println("1000 10F");
        } else if (group == "strip2") {
          Serial.println("1000 20F");
        } else if (group == "pca-group") {
          Serial.println("2000 00F");
        }
      }

    void postSetSource() {
      if (!server.hasArg("plain")) {
        sendResponse("Body not received");
        return;
      }

      sendSetSourceCommand(server.arg("name"));
      sendResponse("Source changed");
    }

      void sendSetSourceCommand(String source) {
        if (source == "serial") {
          Serial.println("0000 11F 77568S");
        } else if (source == "cloud") {
          Serial.println("0000 11F 25683S");
        } else if (source == "local") {
          Serial.println("0000 11F 52547S");
        } 
      }

  void sendResponse(String message) {
    server.send(200, "text/json", String("{ \"message\": \"") + String(message) + String("\" }"));
  }

void loop() {
  server.handleClient();
}
