#include <WebServer.h>
#include "input_control.h"
#include "output_control.h"
#include "http_routes.h"

WebServer server(80); // HTTP server on port 80

// registers all HTTP routes
void registerRoutes(WebServer& server) {
  server.on("/data", [&server]() {
    handleGetData(server);
  });

  server.on("/settings", [&server]() {
    if (server.method() == HTTP_GET) {
      handleGetSettings(server);
    } else if (server.method() == HTTP_PUT) {
      handlePutSettings(server);
    } else {
      server.send(405, "text/plain", "Method Not Allowed");
    }
  });
}

// handles GET /data endpoint

void handleGetData(WebServer& server) {
  String json = "{ \"temps_c\": {";

  for (int i = 0; i < currentTempSensorCount && i < MAX_TEMP_SENSORS; i++) {
    String id = addressToString(sensorAddresses[i]);
    json += "\"" + id + "\": ";
    json += isnan(currentTemperaturesC[i]) ? "null" : String(currentTemperaturesC[i]);
    if (i < currentTempSensorCount - 1 && i < MAX_TEMP_SENSORS - 1) {
      json += ", ";
    }
  }

  json += "} }";
  server.send(200, "application/json", json);
}

// handles GET /settings endpoint
void handleGetSettings(WebServer& server) {
  String json = "{";
  json += "\"inputUpdateIntervalMs\": " + String(inputUpdateIntervalMs) + ",";
  json += "\"outputUpdateIntervalMs\": " + String(outputUpdateIntervalMs) + ",";
  json += "\"hotTempC\": " + String(hotTempC, 2) + ",";
  json += "\"coldTempC\": " + String(coldTempC, 2) + ",";
  json += "\"runFanTempDiffC\": " + String(runFanTempDiffC, 2);
  json += "}";

  server.send(200, "application/json", json);
}

// handles PUT /settings endpoint
void handlePutSettings(WebServer& server) {
  String response = "";

  if (server.hasArg("inputInterval")) {
    inputUpdateIntervalMs = server.arg("inputInterval").toInt();
    response += "input interval set to " + String(inputUpdateIntervalMs) + "\n";
  }

  if (server.hasArg("outputInterval")) {
    outputUpdateIntervalMs = server.arg("outputInterval").toInt();
    response += "output interval set to " + String(outputUpdateIntervalMs) + "\n";
  }

  if (server.hasArg("hotTempC")) {
    hotTempC = server.arg("hotTempC").toFloat();
    response += "hotTempC set to " + String(hotTempC, 2) + "\n";
  }

  if (server.hasArg("coldTempC")) {
    coldTempC = server.arg("coldTempC").toFloat();
    response += "coldTempC set to " + String(coldTempC, 2) + "\n";
  }

  if (server.hasArg("runFanTempDiffC")) {
    runFanTempDiffC = server.arg("runFanTempDiffC").toFloat();
    response += "runFanTempDiffC set to " + String(runFanTempDiffC, 2) + "\n";
  }

  if (response == "") {
    response = "No valid settings provided.\n";
    server.send(400, "text/plain", response);  // Bad request
  } else {
    server.send(200, "text/plain", response);  // OK
  }
}

void httpServerTask(void *pvParameters) {
  while (true) {
    server.handleClient();
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Small delay to yield CPU
  }
}