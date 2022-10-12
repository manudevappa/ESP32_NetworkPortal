# ESP32 Web portal to connect Local Network

Configurations 

- PlatformIO in VSCode
- FreeRTOS
- ESP32 or ESP8266

## Features
- Implemedted on FreeRtos Thread. It wont disturb the actual code Execution running on main loop
- Use NVM to maintain persist connection.
- Host a webpage in AccessPoint mode. When user enters SSID and PASSWORD it Jump to Station mode and try to connect to given network.
- If it fails to connect to given network, It will again fallback to AccessPoint mode so that User can enter correct Credentials.
- If Saved Network not available, It will jump to Access Point mode and User can enter new credentials for the initial setup time. 

## Installation and Configuration
#### install below dependencies to work with this codebase.
Software Requirments :
- [VSCode] - It's recommended to use VScode for development if given a chance between Arduino IDE and VsCode. because it supports PlatformIO! and all made super easy.
- [PlatformIO] - VSCode Extension (Support hundreds of boards)
- libraries - 
    esphome/AsyncTCP-esphome@^1.2.2
	me-no-dev/ESPAsyncTCP@^1.2.2
	ottowinter/ESPAsyncWebServer-esphome@^2.1.0
### Supported Hardware
- [ESP32 or ESP8266]


## How to include this code in your codebase ?
- `Simple` Just Install required libraries and copy paste the [network_config.cpp](#Files) and [network_config.h](#Files) files in your project.
- Include [network_config.h](#network_config.h) in file where you want to access network connection API provided in this codebase.
- First Call [init_eeprom(EEPROM_SIZE);](#init_eeprom) function in setup.
- Then Call [web_init();](#web_init) function 