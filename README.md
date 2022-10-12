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
	- esphome/AsyncTCP-esphome@^1.2.2
	- me-no-dev/ESPAsyncTCP@^1.2.2
	- ottowinter/ESPAsyncWebServer-esphome@^2.1.0
### Supported Hardware
- [ESP32 or ESP8266]


## How to include this code in your codebase ?
- `Simple` Just Install required libraries and copy paste the [network_config.cpp](#Files) and [network_config.h](#Files) files in your project.
- Include [network_config.h](#network_config.h) in file where you want to access network connection API provided in this codebase.
- First Call [init_eeprom(EEPROM_SIZE);](#init_eeprom) function in setup.
- Then Call [web_init();](#web_init) function in setup.
- Connection status can be checked by observing variable `network_connected`. Check this variable in loop. If the variable is `false` then network is not connected. If the variable is `true` then it's successfully connected to given network configurations. 


## How to test the and configure SSID and Password
- After code is uplaoded to ESP32, Search for the WiFi network on Mobile or Laptop. Access Point Name will be the name set on `network_config.cpp` file `const char *ap_ssid` variable.
- Once the WiFi is connected Open any browser and type the IP address 192.168.4.1
- There will be a website contains 2 text boxes.
- Enter the SSID and Password of the Access Point also called as Hotspot, WiFi network login.
- Enter Submit after entering the Details. 
- Once the data is entered, it will trying to connect to that network. If the device is successfully connected `network_connected` variable will become `true` and UART log also display the local IP address.
- If the connection failed due to incorrect SSID or Password or due to any other reasons, it will again jump to Access Point mode and we can connect mobile or laptop to configure again.

## API details
# Files
***network_config.cpp*** :  Consist of logics for network configurations.
***network_config.h*** : Contains web page code and decleration of of public functions for eeprom and network configurations.