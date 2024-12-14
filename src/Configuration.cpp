#include "Configuration.hpp"

static const char *TAG = "Configuration";

extern String cpuId;
extern Configuration Config;

Configuration::Configuration(void)
{
}

Configuration::~Configuration(void)
{
}

void Configuration::get_configs(void)
{
    get_sequrity_cfg();
    delay(50);

    get_wlan_cfg();
    delay(50);
}

void Configuration::get_sequrity_cfg(void)
{
    if (!LITTLEFS.exists(FILE_CFG_SEQURITY))
    {
        default_sequrity_cfg();
    }
    else
    {
        File file = LITTLEFS.open(FILE_CFG_SEQURITY, FILE_READ);

        StaticJsonBuffer<256> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(file);

        if (!root.success())
        {
            default_sequrity_cfg();
        }
        else
        {
            _sequrity.status = root["status"].as<bool>();
            _sequrity.password = root["password"].as<String>();
        }

        file.close();
    }
}

void Configuration::set_sequrity_cfg(void)
{
    if (LITTLEFS.exists(FILE_CFG_SEQURITY))
    {
            if (LITTLEFS.remove(FILE_CFG_SEQURITY))
        {
            ESP_LOGD(TAG, "%s - file deleted", FILE_CFG_SEQURITY);
        }
        else
        {
            ESP_LOGE(TAG, "%s - delete failed", FILE_CFG_SEQURITY);
        }
    }

    File file = LITTLEFS.open(FILE_CFG_SEQURITY, FILE_WRITE);

    if (!file || file.isDirectory())
    {
        ESP_LOGE(TAG, "Failed to create file -  %s", FILE_CFG_SEQURITY);
        return;
    }

    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["status"] = _sequrity.status;
    root["password"] = _sequrity.password;

    if (root.printTo(file) == 0)
    {
        ESP_LOGE(TAG, "Failed to write to file");
    }

    file.close();
}

void Configuration::default_sequrity_cfg(void)
{
    _sequrity.status = true;
    _sequrity.password = "0000";

    set_sequrity_cfg();
}

void Configuration::get_wlan_cfg(void)
{
    if (!LITTLEFS.exists(FILE_CFG_WLAN))
    {
        default_wlan_cfg();
    }
    else
    {
        File file = LITTLEFS.open(FILE_CFG_WLAN, FILE_READ);

        StaticJsonBuffer<256> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(file);

        if (!root.success())
        {
            default_wlan_cfg();
        }
        else
        {
            _wlan.ap_ssid = root["ap_ssid"].as<String>();
            _wlan.ap_password = root["ap_password"].as<String>();
        }

        file.close();
    }
}

void Configuration::set_wlan_cfg(void)
{
    if (LITTLEFS.exists(FILE_CFG_WLAN))
    {
        if (LITTLEFS.remove(FILE_CFG_WLAN))
        {
            ESP_LOGD(TAG, "%s - file deleted", FILE_CFG_WLAN);
        }
        else
        {
            ESP_LOGE(TAG, "%s - delete failed", FILE_CFG_WLAN);
        }
    }

    File file = LITTLEFS.open(FILE_CFG_WLAN, FILE_WRITE);

    if (!file || file.isDirectory())
    {
        ESP_LOGE(TAG, "Failed to create file -  %s", FILE_CFG_WLAN);
        return;
    }

    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["ap_ssid"] = _wlan.ap_ssid;
    root["ap_password"] = _wlan.ap_password;

    if (root.printTo(file) == 0)
    {
        ESP_LOGE(TAG, "Failed to write to file");
    }

    file.close();
}

void Configuration::default_wlan_cfg(void)
{
    _wlan.ap_ssid = AP_SSID;
    _wlan.ap_password = AP_PASSWORD;

    set_wlan_cfg();
}

void Configuration::restore_all()
{
    default_sequrity_cfg();
    delay(50);

    default_wlan_cfg();
    delay(50);
}

void Configuration::listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    File root = fs.open(dirname);

    if (!root)
    {
        ESP_LOGV(TAG, "%s - failed to open directory", dirname);
        return;
    }

    if (!root.isDirectory())
    {
        ESP_LOGV(TAG, "%s - not a directory", dirname);
        return;
    }

    File file = root.openNextFile();

    while (file)
    {
        if (!file.isDirectory())
        {
            ESP_LOGV(TAG, "FILE: %s, SIZE: %d", file.name(), file.size());
        }

        file = root.openNextFile();
    }
}
