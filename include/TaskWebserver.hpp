#pragma once

#include "main.hpp"

#include "ArduinoJson.h"
#include "FS.h"
#include "LITTLEFS.h"
#include "GyverPortal.h"

void Index_Page(void);
void Security_Page(void);
void Network_Page(void);
void Update_Page(void);

void action(void);
void build(void);

void TaskWebserver(void *pvParameters);
