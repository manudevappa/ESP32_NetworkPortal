/**
 * @file main.cpp
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Implementation Access point Mode and Station Mode
 *        Using FreeRtos
 * @version 0.1
 * @date 12-10-2022
 * @copyright Copyright (c) 2022
 */

/*Include this file in your your code*/
#include "network_config.h"
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  init_eeprom(EEPROM_SIZE);
  /* Uncomment for Testing, 
    it will erase eeprom so that it can start
   from Accss point mode everytime it restarts  */
  EEPROM.write(0,0);
  web_init();
}

void loop()
{ 
  if(network_connected){
    Serial.println("network is connected");
  }
  else{
    Serial.println("network is not connected");
  }
  delay(1000);
}