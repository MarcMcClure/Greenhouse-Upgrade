#pragma once

#include <WebServer.h>

void registerRoutes(WebServer& server);
void handleGetSettings(WebServer& server);
void handleGetData(WebServer& server);
void handlePutSettings(WebServer& server);