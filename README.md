# ESP32 Web portal to connect Local Network

Configurations 

- PlatformIO in VSCode
- FreeRTOS
- ESP32

## Features

- Use NVM to maintain persist connection.
- Host a webpage in AccessPoint mode. When user enters SSID and PASSWORD it Jump to Station mode and try to connect to given network
- If it fails to connect to given network, It will again fallback to AccessPoint mode so that User can enter correct Credentials.
- If Hotspot or Network went off, ESP23 will try to connect to previously connected network for every 3 minutes and inbetween it will be on Access Point mode.


## Installation and Configuration

You can install below dependencies to work with this codebase.

Software Requirments :
- [VSCode] - One of my Fav Editor, It's recommended to use VScode because it supports PlatformIO!
- [PlatformIO] - It's a VSCode Extension
- libraries - There are two libraries you need to install. Please search on PlatformIO Library section. Type header file name without .h extention you will get latest Library file and you can add that to current project.

Hardware
- [ESP32 or ESP8266]

