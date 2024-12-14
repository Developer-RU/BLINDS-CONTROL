#pragma once

#include "Arduino.h"


#define UART                        Serial
#define BAUNDRATE                   115200

#define FIRMWARE_NAME               "ЖАЛЮЗИ"
#define MANUFACTURE                 "MSVM.RU"
#define MODEL                       "BLINDS-CONTROL"
#define VERSION                     "1.00.00"
#define REVISION                    "1"


#define RELAY_UP                      32
#define RELAY_DOWN                    33
#define RELAY_ONE                     25
#define RELAY_TWO                     26

#define SENSOR_UP                     22
#define SENSOR_DOWN                   23

#define ON                            HIGH
#define OFF                           LOW

#define FILE_CFG_SEQURITY           "/cfg_sequrity.json"
#define FILE_CFG_WLAN               "/cfg_wlan.json"


#define STACK_SIZE_PROGRESS         8192
#define STACK_SIZE_WEBSERVER        8192


#define AP_SSID                     "NetLAN"
#define AP_PASSWORD                 ""
