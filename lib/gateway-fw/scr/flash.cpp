/**
 * @file flash.cpp
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Non Volatile Memory for Persistent Connection
 * @version 0.1
 * @date 30-12-2021
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "flash.h"
#include "deviceCommon.h"
#include <stdint.h>
#include <Arduino.h>

/*=============================================================================
 *                                 Defines
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
        Serial.println("Failed to initialise EEPROM");
        Serial.println("Restarting...");
        delay(1000);
        ESP.restart();
    }
}

int eeprom_write_credentials(const char *ssid_write, const char *password_write)
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

int eeprom_read_credentials(char *ssid_read_eeprom, char *pass_read_eeprom)
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