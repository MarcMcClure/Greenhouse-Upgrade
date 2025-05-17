#include <WebServer.h>
#include "temp_control.h"
#include "http_routes.h"

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
  String json = "{ \"temp_c\": " + String(currentTempC, 2) + " }";
  server.send(200, "application/json", json);
}

// handles GET /settings endpoint
void handleGetSettings(WebServer& server) {
  String json = "{";
  json += "\"updateIntervalMs\": " + String(updateIntervalMs) + ",";
  json += "\"criticalTempC\": " + String(criticalTempC, 2);
  json += "}";

  server.send(200, "application/json", json);
}


// handles PUT /settings endpoint
void handlePutSettings(WebServer& server) {
  String response = "";

  if (server.hasArg("interval")) {
    updateIntervalMs = server.arg("interval").toInt();
    response += "interval set to " + String(updateIntervalMs) + "\n";
  }

  if (server.hasArg("criticalTempC")) {
    criticalTempC = server.arg("criticalTempC").toFloat();
    response += "criticalTempC set to " + String(criticalTempC, 2) + "\n";
  }

  if (response == "") {
    response = "No valid settings provided.\n";
    server.send(400, "text/plain", response);  // Bad request
  } else {
    server.send(200, "text/plain", response);  // OK
  }
}