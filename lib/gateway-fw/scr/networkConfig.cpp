/**
 * @file networkConfig.cpp
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Network Code
 * @version 0.1
 * @date 30-12-2021
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "networkConfig.h"
#include "flash.h"
#include <Arduino.h>
#include <string>
#include <cstring>
/*=============================================================================
 *                                 Defines
 *==============================================================================*/

static TaskHandle_t NW_Handler = NULL;

#define CPU_CORE 1

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char *ap_ssid = "Gateway_A1";
const char *ap_password = "12345678";

const char *get_ssid = "ssid";
const char *get_password = "password";

char ssid_try_connect[20];
char password_try_connect[20];

AsyncWebServer server(80);

// Globals
static SemaphoreHandle_t get_form_data;

bool network_connected = false;

/*=============================================================================
 *                             Private Functions
 *==============================================================================*/

static int access_point_init();

static int set_ssid_password(const char *ssid, const char *password)
{
    strcpy(ssid_try_connect, ssid);
    strcpy(password_try_connect, password);
    return ALL_OK;
}

/**
 * @brief Connection Establishment to given SSID and Password
 *
 * @param ssid SSID of new Connection
 * @param password Password of new Connection
 *
 * @return WL_CONNECTED on success, WL_CONNECT_FAILED on failed
 *
 */
static int connect_wifi(const char *ssid, const char *password)
{
    int count = 0;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.printf("\n Connect to SSID: %s Password: %s\n", ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        count++;
        Serial.print('.');
        delay(1000);
        if (count > 30)
        {
            Serial.println("Falied to Conenct");
            WiFi.disconnect();
            vTaskDelay(500 / portTICK_PERIOD_MS);
            return WL_CONNECT_FAILED;
        }
        TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
        TIMERG0.wdt_feed = 1;
        TIMERG0.wdt_wprotect = 0;
    }
    Serial.println(WiFi.localIP());
    network_connected = true;
    return ALL_OK;
}

static void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

static int access_point_init()
{

    WiFi.mode(WIFI_STA);
    WiFi.softAP(ap_ssid, ap_password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // DNS Server Initializing
    if (!MDNS.begin("GATEWAY"))
    {
        Serial.println("Error starting mDNS");
        return DNS_ERROR;
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
    // Display FORM Page
    request->send_P(200, "text/html", index_html); });

    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  request->send(200, "text/html", "Trying to Connect to Network");

                  String inputMessage1;
                  String inputMessage2;
                  if (request->hasParam(get_ssid))
                  {
                      inputMessage1 = request->getParam(get_ssid)->value();
                      if (request->hasParam(get_password))
                      {
                          inputMessage2 = request->getParam(get_password)->value();
                      }
                  }
                  else
                  {
                      Serial.println("No Message");
                  }

                    const char *_getssid;
                    const char *_getpassword;
                    Serial.println(inputMessage1);
                    Serial.println(inputMessage2);
                    _getssid = inputMessage1.c_str();
                    _getpassword = inputMessage2.c_str();
                    Serial.printf("%s", _getssid);
                    Serial.printf("%s\n", _getpassword);
                    xSemaphoreGive(get_form_data);
                    set_ssid_password(_getssid, _getpassword); });

    server.onNotFound(notFound);
    server.begin();
    Serial.println("Server Begin");
    return ALL_OK;
}

void connection_control(void *parameter)
{

    while (1)
    {

        while (network_connected != true)
        {

            if (EEPROM.read(0) != 13)
            {
                if (access_point_init() != ALL_OK)
                {
                    Serial.println("Cannot initialize Access Point");
                }
                Serial.println("Waiting for Semaphore");
                xSemaphoreTake(get_form_data, portMAX_DELAY);
                Serial.println("Got Semaphor");
                // Serial.printf("Connect to SSID %s Password %s", ssid_try_connect, password_try_connect);
                int err = connect_wifi(ssid_try_connect, password_try_connect);

                if (err != ALL_OK)
                {
                    Serial.println("Error connecting to Given SSID and password");
                    Serial.println("Server End");
                    server.end();
                }

                else
                {
                    int err = eeprom_write_credentials(ssid_try_connect, password_try_connect);
                    if (err != ALL_OK)
                    {
                        Serial.println("Error while writing credentials");
                    }
                    server.end();
                    Serial.println("Server End");
                }
            }
            else
            {
                Serial.println("Trying to Connected already Connected Network");
                char ssid_read_eeprom[20];
                char pass_read_eeprom[20];

                int err = eeprom_read_credentials(ssid_read_eeprom, pass_read_eeprom);
                if (err == ALL_OK)
                {
                    // Serial.printf("SSID %s, Password: %s\n", ssid_read_eeprom, pass_read_eeprom);
                    err = connect_wifi(ssid_read_eeprom, pass_read_eeprom);
                    if (err != ALL_OK)
                    {
                        while (1)
                        {

                            access_point_init();
                            Serial.printf("Retry after %d\n", RETRY_DELAY_MSEC);
                            err = xSemaphoreTake(get_form_data, (TickType_t)RETRY_DELAY_MSEC);
                            if (err != pdFALSE)
                            {

                                err = connect_wifi(ssid_try_connect, password_try_connect);

                                if (err != ALL_OK)
                                {
                                    Serial.println("Error connecting to Given SSID and password");
                                    Serial.println("Server End");
                                    server.end();
                                }

                                else
                                {
                                    err = eeprom_write_credentials(ssid_try_connect, password_try_connect);
                                    if (err != ALL_OK)
                                    {
                                        Serial.println("Error while writing credentials");
                                    }
                                    server.end();
                                    Serial.println("Server End");
                                }
                            }

                            Serial.println("Trying to connect existing network");
                            eeprom_read_credentials(ssid_read_eeprom, pass_read_eeprom);
                            err = connect_wifi(ssid_read_eeprom, pass_read_eeprom);
                            if (err != WL_CONNECT_FAILED)
                            {
                                Serial.println("Suspend Task");
                                vTaskSuspend(NW_Handler);
                            }
                        }
                    }
                    Serial.println("Suspend Task");
                    vTaskSuspend(NW_Handler);
                }
            }
        }
    }
    // vTaskSuspend(NW_Handler);
}

/*=============================================================================
 *                             Public Functions
 *==============================================================================*/

void web_init()
{
    // Function to trigger once the Task has been started
    get_form_data = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(
        connection_control,
        "Task 2",    // name for the Task
        40240,       // Stack Size for the task stack
        NULL,        // Parameters to pass to function
        1,           // Task Priority 1 is lowest 25 is highest
        &NW_Handler, // Task Handler to manage this task from other task
        CPU_CORE);   // CPU core to RUN the code
}
