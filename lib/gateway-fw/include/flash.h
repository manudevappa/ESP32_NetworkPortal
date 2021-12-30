/**
 * @file flash.h
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Source code for the flash
 * @version 0.1
 * @date 30-12-2021
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef FLASH_H
#define FLASH_H
/*=============================================================================
 *                                 Includes
 *==============================================================================*/

#include "config.h"
#include <Arduino.h>
#include <EEPROM.h>


#define EEPROM_SIZE 50


void init_eeprom(int);


/**
 * @brief Write WiFi configuration in EEPROM
 *
 * @param ssid SSID get from the user
 * @param password Password get from the User
 * @return int ALL_OK if successful
 */

 int eeprom_write_credentials(const char *ssid, const char *password);



/**
 * @brief Read WiFi configuration from EEPROM
 *
 * @param ssid Address to hold SSID
 * @param password Address to hold Password
 * @return int ALL_OK if successful
 */
int eeprom_read_credentials(char *, char *);

#endif