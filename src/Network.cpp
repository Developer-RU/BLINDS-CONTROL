#include "Network.hpp"

static const char *TAG = "Network";

extern State state;
extern Command command;
extern Relay relay;
extern String cpuId;
extern Configuration Config;

void wlan_init(void)
{
    WiFi.mode(WIFI_AP);

    if (Config._wlan.ap_ssid != "")
    {
        WiFi.softAP(Config._wlan.ap_ssid, Config._wlan.ap_password);
    }
    else
    {
        WiFi.softAP(AP_SSID, AP_PASSWORD);
    }
}
