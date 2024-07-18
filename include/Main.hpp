#pragma once 

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "FS.h"
#include "SPIFFS.h"

#include "AppConfig.hpp"

bool getAuth();
bool setAuth(bool state);
bool isAuth(void);

void getCpuID(void);
void setupServer(void);
void setup(void);
void loop(void);
