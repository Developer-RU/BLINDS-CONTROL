#pragma once 

#include "esp32-hal-log.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "AppConfig.hpp"
#include "AppEnums.hpp"
#include "Global.hpp"

#include "Configuration.hpp"
#include "TaskWebserver.hpp"
#include "Network.hpp"
#include "TaskProgress.hpp"


void get_cpuID(void);
void gpio_init();
void setup(void);
void loop(void);
