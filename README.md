# ESP32 Web portal to connect Local Network

Configurations 

- PlatformIO in VSCode
- FreeRTOS
- ESP32 or ESP8266

## Features
- Implemented on FreeRtos Thread. It won't disturb the actual code Execution running on the main loop
- Use NVM to maintain a persistent connection.
- Host a webpage in AccessPoint mode. When the user enters SSID and PASSWORD it Jumps to Station mode and tries to connect to the given network.
- If it fails to connect to a given network, It will again fall back to AccessPoint mode so that the User can enter the correct Credentials.
- If Saved Network is not available, It will jump to Access Point mode and the User can enter new credentials for the initial setup time. 

## Installation and Configuration
#### install the below dependencies to work with this codebase.
Software Requirments :
- [VSCode] - It's recommended to use VScode for development if given a chance between Arduino IDE and VsCode. because it supports PlatformIO! and all made super easy.
- [PlatformIO] - VSCode Extension (Support hundreds of boards)
- libraries - 
	- esphome/AsyncTCP-esphome@^1.2.2
	- me-no-dev/ESPAsyncTCP@^1.2.2
	- ottowinter/ESPAsyncWebServer-esphome@^2.1.0
### Supported Hardware
- [ESP32 or ESP8266]


## How to include this code in your codebase?
- `Simple` Just Install required libraries and copy paste the [network_config.cpp](#Files) and [network_config.h](#Files) files in your project.
- Include [network_config.h](#network_config.h) in file where you want to access network connection API provided in this codebase.
- First Call [init_eeprom(EEPROM_SIZE);](#init_eeprom) function in setup.
- Then Call [web_init();](#web_init) function in setup.
- Connection status can be checked by observing the variable `network_connected`. Check this variable in the loop. If the variable is `false` then the network is not connected. If the variable is `true` then it's successfully connected to given network configurations. 


## How to test and configure SSID and Password
- After the code is uploaded to ESP32, Search for the WiFi network on your Mobile or Laptop. Access Point Name will be the name set on `network_config.cpp` file `const char *ap_ssid` variable.
- Once the WiFi is connected Open any browser and type the IP address 192.168.4.1
- There will be a website containing 2 text boxes.
- Enter the SSID and Password of the Access Point also called Hotspot, WiFi network login.
- Enter Submit after entering the Details. 
- Once the data is entered, it will be trying to connect to that network. If the device is successfully connected `network_connected` variable will become `true` and the UART log also displays the local IP address.
- If the connection failed due to an incorrect SSID or Password or due to any other reasons, it will again jump to Access Point mode and we can connect mobile or laptop to configure again.

## API details
# Files
- ***network_config.cpp*** : Consists of logic for network configurations.
- ***network_config.h***: Contains web page code and declaration of public functions for EEPROM and network configurations.