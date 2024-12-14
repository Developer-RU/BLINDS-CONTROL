#pragma once

#include "main.hpp"

#include "ArduinoJson.h"
#include "FS.h"
#include "LITTLEFS.h"

struct Config_SYSTEM
{
    unsigned long total_money = 0;
    unsigned long total_discount = 0;
    unsigned long total_clients = 0;
    unsigned long money = 0;
};

struct Config_SEQURITY
{
    bool status = false;
    String password = "";
};

struct Config_WLAN
{
    String ap_ssid = "";
    String ap_password = "";
};


struct Config_PROGRESS
{
    int pause_time = 0; // Установленное время паузы - в процессе работы
};

class Configuration
{
public:
    Config_SYSTEM _system;
    Config_SEQURITY _sequrity;
    Config_WLAN _wlan;

    Config_PROGRESS _progress;


    Configuration(void);

    virtual ~Configuration(void);
    void get_configs(void);

    void get_sequrity_cfg(void);
    void set_sequrity_cfg(void);
    void default_sequrity_cfg(void);

    void get_wlan_cfg(void);
    void set_wlan_cfg(void);
    void default_wlan_cfg(void);

    void restore_all(void);
    void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
    void readFile(fs::FS &fs, const char *path);
};
