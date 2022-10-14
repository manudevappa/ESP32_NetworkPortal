/**
 * @file networkConfig.cpp
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Network Code
 * @version 0.1
 * @date 12-10-2022
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "network_config.h"
#include <Arduino.h>
#include <string>
#include <cstring>
/*=============================================================================
 *                                 Defines
 *==============================================================================*/
// Task Handler 
static TaskHandle_t NW_Handler = NULL;

// This Task runs on CPU 1
#define CPU_CORE 1

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char *ap_ssid = "esp32_gateway";     // Replace Desired Access point name
const char *ap_password = "12345678";      // Replace Desired Access point password

// Do not modify this. It's input field comparator
const char *get_ssid = "ssid";
const char *get_password = "password";

// Maximum SSID length according to the documentation of Standard
char ssid_try_connect[32];
char password_try_connect[20];  // modify password length if needed

AsyncWebServer server(80);

// Semaphor to notify main thread
static SemaphoreHandle_t network_sem;

bool network_connected = false;

/*=============================================================================
 *                             Private Functions
 *==============================================================================*/


static int eeprom_write_credentials(const char *ssid_write, const char *password_write)
{
    uint8_t connected = 13;
    EEPROM.write(0, connected);
    int address = 1;
    EEPROM.writeString(address, ssid_write);
    address += strlen(ssid_write) + 1;
    EEPROM.writeString(address, password_write);
    EEPROM.commit();

    vTaskDelay(300 / portTICK_PERIOD_MS);
    // Serial.println("Write EEPROM Completed");
    return ALL_OK;
}

static int eeprom_read_credentials(char *ssid_read_eeprom, char *pass_read_eeprom)
{
    int address = 1;

    String ssid_read;
    ssid_read = EEPROM.readString(address);
    address += ssid_read.length() + 1;

    String pass_read;
    pass_read = EEPROM.readString(address);

    strcpy(ssid_read_eeprom, ssid_read.c_str());
    strcpy(pass_read_eeprom, pass_read.c_str());
    vTaskDelay(300 / portTICK_PERIOD_MS);

    // Serial.printf("SSID %s : Pass %s",ssid_read_eeprom,pass_read_eeprom);
    // Serial.printf("Read EEPROM Completed\n");
    return ALL_OK;
}

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
    Serial.printf("Connect to SSID: %s Password: %s\n", ssid, password);
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
    Serial.print("\nSuccesfully connected : Local IP Address: ");
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
    if (!MDNS.begin("web_login"))
    {
        Serial.println("Error starting mDNS");
        return DNS_ERROR;
    }
    /* Passing homepage html code to the server_name / URL */
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        { 
            // Display FORM Page
            request->send_P(200, "text/html", index_html); 
        });

    /* Passing get html code and response C code to /get
    This page will get triggered when User enterted the 
    ssid and password in webpage and click submit
     */
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
            /* Set SIID and Password in Global variable's */
            set_ssid_password(_getssid, _getpassword); 
            /* Passing the message to main thread that New ssid and pass is available  */
            xSemaphoreGive(network_sem);                
        });
    
    server.onNotFound(notFound);
    server.begin();             // Begin the Server
    Serial.print("Server Begin Search for ");
    Serial.println(ap_ssid);
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
                xSemaphoreTake(network_sem, portMAX_DELAY);
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
                            Serial.printf(" Jumped to Access point mode : Retry connecting to saved n/w after %d\n", RETRY_DELAY_MSEC);
                            err = xSemaphoreTake(network_sem, (TickType_t)RETRY_DELAY_MSEC);
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
}

/*=============================================================================
 *                             Public Functions
 *==============================================================================*/

/**
 * @brief Initializing EEPROM configuration
 *
 * @param size size of the memory in bytes
 *
 */
void init_eeprom(int size)
{
    if (!EEPROM.begin(size))
    {
        Serial.println("Failed to initialise EEPROM Restarting");
        delay(1000);
        ESP.restart();
    }
}

void web_init()
{
    // Function to trigger once the Task has been started
    network_sem = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(
        connection_control,
        "Task 2",    // name for the Task
        40240,       // Stack Size for the task stack
        NULL,        // Parameters to pass to function
        1,           // Task Priority 1 is lowest 25 is highest
        &NW_Handler, // Task Handler to manage this task from other task
        CPU_CORE);   // CPU core to RUN the code
}

