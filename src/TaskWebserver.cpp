#include "TaskWebserver.hpp"

static const char *TAG = "TaskWebserver";

extern State state;
extern Command command;
extern Relay relay;
extern String cpuId;
extern Configuration Config;

GyverPortal portal(&LITTLEFS);

void Index_Page(void)
{
    GP.BREAK();
    GP.TITLE(FIRMWARE_NAME, "", GP_DEFAULT);
    GP.LABEL("КОНТРОЛЛЕР УПРАВЛЕНИЯ");

    GP.BREAK();
    GP.BREAK();
    GP.HR();
    GP.BREAK();

    GP.JQ_SUPPORT();
    GP.JQ_UPDATE_BEGIN(2000);

    M_BLOCK_THIN_TAB(
        "",
        M_BOX(
            GP_JUSTIFY,
            GP.BUTTON("OPEN", "Открыть"););

        M_BOX(
            GP_JUSTIFY,
            GP.BUTTON("DISABLE", "Остановить"););

        M_BOX(
            GP_JUSTIFY,
            GP.BUTTON("CLOSE", "Закрыть");););

    GP.BREAK();

    M_BLOCK_THIN_TAB(
        "",
        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.LABEL("Освещение"); GP.SWITCH("LIGHT", relay.one););););

    M_BLOCK_THIN_TAB(
        "",
        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.LABEL("Сигнализация"); GP.SWITCH("WATCHDOG", relay.two););););

    GP.JQ_UPDATE_END();

    GP.BREAK();
    GP.HR();
    GP.BREAK();

    GP.LABEL("Copyright © Developer-RU");
    GP.BREAK();
    GP.LABEL("2024");

    GP.BREAK();
}

void Security_Page(void)
{
    GP.BREAK();
    GP.FORM_BEGIN("/security");

    M_BLOCK_THIN_TAB(
        "",
        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.LABEL("Запрашивать пароль"); GP.SWITCH("STATUS", Config._sequrity.status););););

    M_BLOCK_THIN_TAB(
        "",

        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.LABEL("Введите новый пароль");););

        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.TEXT("PASSWORD", "новый пароль", Config._sequrity.password);););

        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.LABEL("Повторите новый пароль");););

        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.TEXT("TWOPASSWORD", "новый пароль", Config._sequrity.password););););

    GP.SUBMIT("Сохранить");
    GP.FORM_END();
    GP.BREAK();

    GP.HR();
    GP.BREAK();

    GP.LABEL("Copyright © Developer-RU");
    GP.BREAK();
    GP.LABEL("2024");

    GP.BREAK();
}

void Network_Page(void)
{
    GP.BREAK();
    GP.FORM_BEGIN("/network");

    M_BLOCK_THIN_TAB(
        "",
        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.LABEL("SSID");););

        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.TEXT("SSID", "wlan ssid", Config._wlan.ap_ssid);););

        GP.BREAK();

        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.LABEL("Pasword");););

        M_BOX(
            GP_JUSTIFY,
            M_BOX(GP.TEXT("PASSWORD", "wlan password", Config._wlan.ap_password););););

    GP.SUBMIT("Сохранить");
    GP.FORM_END();
    GP.BREAK();

    GP.HR();
    GP.BREAK();

    GP.LABEL("Copyright © Developer-RU");
    GP.BREAK();
    GP.LABEL("2024");

    GP.BREAK();
}

void Update_Page(void)
{
    GP.BREAK();
    GP.FORM_BEGIN("/update");

    M_BLOCK_THIN_TAB(
        "",
        GP.BUTTON_LINK("/ota_update", "Обновление вручную"););

    GP.SUBMIT("Сохранить");
    GP.FORM_END();
    GP.BREAK();

    GP.HR();
    GP.BREAK();

    GP.LABEL("Copyright © Developer-RU");
    GP.BREAK();
    GP.LABEL("2024");

    GP.BREAK();
}

void action(void)
{
    if (portal.click())
    {
        if (portal.click("CLOSE"))
        {
            command = Command_Down;
            UART.println("CLOSE");
        }
        
        if (portal.click("DISABLE"))
        {
            command = Command_Stop;
            UART.println("DISABLE");
        }
        
        if (portal.click("OPEN"))
        {
            command = Command_Up;
            UART.println("OPEN");
        }
        
        if (portal.clickBool("LIGHT", relay.one))
        {
            UART.println("LIGHT " + relay.one ? "ON" : "OFF");
        }
        
        if (portal.clickBool("WATCHDOG", relay.two))
        {
            UART.println("WATCHDOG " + relay.two ? "ON" : "OFF");
        }
    }
    
    if (portal.form())
    {
        if (portal.form("/security"))
        {
            bool status = portal.getBool("STATUS");
            String password = portal.getString("PASSWORD");
            String two_password = portal.getString("TWOPASSWORD");

            password.trim();
            two_password.trim();

            if (password == two_password)
            {
                Config._sequrity.status = status;
                if (password != "" && password.length() < 65)
                    Config._sequrity.password = password;

                Config.set_sequrity_cfg();
            }
        }
        else if (portal.form("/network"))
        {
            String ap_ssid = portal.getString("SSID");
            String ap_password = portal.getString("PASSWORD");

            ap_ssid.trim();
            ap_password.trim();

            if (ap_ssid != "" && ap_ssid.length() < 65)
                Config._wlan.ap_ssid = ap_ssid;
            if (ap_password != "" && ap_password.length() < 65)
                Config._wlan.ap_password = ap_password;

            Config.set_wlan_cfg();
        }
    }
}

void build(void)
{
    ESP_LOGV(TAG, "%s", "Build page start");

    // portal.clearCache();

    GP.BUILD_BEGIN();
    // GP.THEME(STYLE_THEME);
    GP.THEME(GP_DARK);

    GP.UI_MENU("");

    // GP.UI_MENU(MODEL);

    GP.UI_LINK("/", "Главная");
    GP.HR();
    GP.UI_LINK("/security", "Безопасность");
    GP.UI_LINK("/network", "Настройки WiFi");
    GP.UI_LINK("/update", "Обновление ПО");
    GP.HR();
    GP.UI_LINK("/restart", "Перезагрузить");

    GP.UI_BODY(); // начать главную колонку

    if (portal.uri("/"))
    {
        Index_Page();
    }
    else if (portal.uri("/security"))
    {
        Security_Page();
    }
    else if (portal.uri("/network"))
    {
        Network_Page();
    }
    else if (portal.uri("/update"))
    {
        Update_Page();
    }
    else if (portal.uri("/restart"))
    {
        ESP.restart();
        return;
    }

    GP.UI_END();
    GP.BUILD_END();
    ESP_LOGV(TAG, "%s", "Build page end");
}

void TaskWebserver(void *pvParameters)
{
    (void)pvParameters;

    ESP_LOGV(TAG, "%s", "TaskWebserver start");
    ESP_LOGV(TAG, "%s", "Init WebServer");

    portal.attachBuild(build);
    portal.attach(action);

    portal.enableOTA(); // без пароля
    // portal.enableOTA("admin", "0000");  // с паролем

    if (Config._sequrity.status)
    {
        portal.enableAuth("admin", Config._sequrity.password.c_str());
    }
    else
    {
        // portal.disableAuth();
    }

    portal.downloadAuto(true);
    portal.start();

    // portal.detachError();

    ESP_LOGV(TAG, "%s", "WebServer started");

    for (;;)
    {
        try
        {
            portal.tick();
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "%s", "Crash TaskWebserver");
        }

        vTaskDelay(5);
    }

    ESP_LOGE(TAG, "%s", "TaskWebserver end");
    vTaskDelete(NULL);
}
