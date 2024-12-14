#include "TaskProgress.hpp"

static const char *TAG = "TaskProgress";

extern State state;
extern Command command;
extern Relay relay;
extern String cpuId;
extern Configuration Config;

void TaskProgress(void *pvParameters)
{
    (void)pvParameters;

    ESP_LOGV(TAG, "%s", "TaskProgress start");

    vTaskDelay(500);

    for (;;)
    {
        if (state == State_Ready)
        {
            if (command == Command_Stop)
            {
                if (digitalRead(RELAY_UP) == ON)
                {
                    digitalWrite(RELAY_UP, OFF);
                }

                if (digitalRead(RELAY_DOWN) == ON)
                {
                    digitalWrite(RELAY_DOWN, OFF);
                }
            }

            if (command == Command_Down)
            {
                if (digitalRead(RELAY_UP) == ON)
                {
                    digitalWrite(RELAY_UP, OFF);
                }

                vTaskDelay(100);

                if (digitalRead(SENSOR_DOWN) == ON)
                {
                    digitalWrite(RELAY_DOWN, ON);
                }
                else
                {
                    command = Command_Stop;
                }
            }

            if (command == Command_Up)
            {
                if (digitalRead(RELAY_DOWN) == ON)
                {
                    digitalWrite(RELAY_DOWN, OFF);
                }

                vTaskDelay(100);

                if (digitalRead(SENSOR_UP) == ON)
                {
                    digitalWrite(RELAY_UP, ON);
                }
                else
                {
                    command = Command_Stop;
                }
            }

            if (digitalRead(RELAY_ONE) != relay.one)
            {
                digitalWrite(RELAY_ONE, relay.one);
            }

            if (digitalRead(RELAY_TWO) != relay.two)
            {
                digitalWrite(RELAY_TWO, relay.two);
            }
        }
        else
        {
            command = Command_Stop;

            digitalWrite(RELAY_UP, OFF);
            digitalWrite(RELAY_DOWN, OFF);
        }

        vTaskDelay(50);
    }

    ESP_LOGE(TAG, "%s", "TaskProgress end");
    vTaskDelete(NULL);
}
