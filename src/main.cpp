/**
 * @file main.cpp
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Implementation Access point Mode and Station Mode
 *        Using FreeRtos
 * @version 0.1
 * @date 30-12-2021
 * @copyright Copyright (c) 2021
 */

#include "gateway.h"
#include <Arduino.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


void setup()
{
  Serial.begin(115200);
  init_eeprom(EEPROM_SIZE);
  /* Uncomment for Testing, 
    it will erase eeprom so that it can start
   from Accss point mode everytime it restarts  */
  
  // EEPROM.write(0,0);
  watchdog_init_and_start();
  web_init();
}

void loop()
{

}