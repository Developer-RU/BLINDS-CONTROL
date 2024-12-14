#include <Main.hpp>

static const char *TAG = "Main";

extern State state;
extern Command command;
extern Relay relay;
extern String cpuId;
extern Configuration Config;
 
void get_cpuID(void)
{
    uint64_t chipid = ESP.getEfuseMac();
    cpuId = String((uint16_t)(chipid>>32)) + String((uint32_t)chipid);  
}

void gpio_init()
{
    pinMode(SENSOR_UP, INPUT_PULLUP);
    pinMode(SENSOR_DOWN, INPUT_PULLUP);

    pinMode(RELAY_UP, OUTPUT);
    digitalWrite(RELAY_UP, OFF);

    pinMode(RELAY_DOWN, OUTPUT);
    digitalWrite(RELAY_DOWN, OFF);

    pinMode(RELAY_ONE, OUTPUT);
    digitalWrite(RELAY_ONE, OFF);

    pinMode(RELAY_TWO, OUTPUT);
    digitalWrite(RELAY_TWO, OFF);   
}

void setup(void)
{
    state = State_Init;

    gpio_init();

    UART.begin(BAUNDRATE, SERIAL_8N1);
    // UART.setDebugOutput(0);

    get_cpuID();

    ESP_LOGV (TAG, "--------------------------------------------------------------------------------------------------------------------------------------------");
    ESP_LOGI (TAG, "FIRMWARE NAME   - : %s", FIRMWARE_NAME);
    ESP_LOGI (TAG, "SERIAL NUMBER   - : %s", cpuId.c_str());
    ESP_LOGI (TAG, "VERSION NAME    - : %s", VERSION);
    ESP_LOGI (TAG, "REVISION        - : %s", REVISION);
    ESP_LOGI (TAG, "MANUFACTURE     - : %s", MANUFACTURE);
    ESP_LOGI (TAG, "MODEL           - : %s", MODEL);
    ESP_LOGV (TAG, "--------------------------------------------------------------------------------------------------------------------------------------------");

    ESP_LOGD (TAG, "%s", "LittleFS Mount start" );

    if(!LITTLEFS.begin(true))
    {
        ESP_LOGE (TAG, "LittleFS Mount failed");
        return;
    }
    else
    {
        ESP_LOGD (TAG, "LittleFS Mount conplete");
    }

    ESP_LOGD (TAG, "%s", "LittleFS Load config" );

    Config.get_configs();

    ESP_LOGV (TAG, "--------------------------------------------------------------------------------------------------------------------------------------------");
    ESP_LOGV (TAG, "Files configurations from Flash Ram");
    Config.listDir(LITTLEFS, "/", 3);
    ESP_LOGV (TAG, "--------------------------------------------------------------------------------------------------------------------------------------------");

    ESP_LOGD (TAG, "%s", "Start network" );

    wlan_init();

    ESP_LOGD (TAG, "%s", "Start tasks" );

    xTaskCreate(TaskWebserver,  "TaskWebserver",    STACK_SIZE_WEBSERVER,   NULL, 1, NULL);
    // xTaskCreate(TaskRelays,     "TaskRelays",       STACK_SIZE_RELAY,       NULL, 2, NULL);
    xTaskCreate(TaskProgress,   "TaskProgress",     STACK_SIZE_PROGRESS,    NULL, 2, NULL);

    state = State_Ready;

    ESP_LOGD (TAG, "%s", "Started OK" );
}

void loop(void)
{
  
}
