/**
 * @file networkConfig.h
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Source of network configuration
 * @version 0.1
 * @date 30-12-2-21
 *
 * @copyright Copyright (c) 2021
 *
 *  Reference
 * https://randomnerdtutorials.com/esp32-esp8266-input-data-html-form/ Reference
 * : https://github.com/techiesms/NodeMCU-ESP32-Series
 */

/*=============================================================================
 *                                 Includes
 *==============================================================================*/

#include "deviceCommon.h"

/*
 * WiFi Library Include
 *
 */
#ifdef ESP32
#include <AsyncTCP.h>
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

#include "soc/timer_group_reg.h"
#include "soc/timer_group_struct.h"

#define RETRY_DELAY_MSEC 120000

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html> 
<html> 
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title> Web Portal </title>
<style> 
Body {
  font-family: Calibri, Helvetica, sans-serif;
  background-color: pink;
}
button { 
       background-color: #4CAF50; 
       width: 100%;
        color: black; 
        padding: 15px; 
        margin: 10px 0px; 
        border: none; 
        cursor: pointer;

         } 
 form { 
        border: 3px solid #f1f1f1; 
    } 
 input[type=text], input[type=password] { 
        width: 100%; 
        margin: 8px 0;
        padding: 12px 20px; 
        display: inline-block; 
        border: 2px solid green; 
        box-sizing: border-box; 
    }

 .container { 
        padding: 25px; 
        background-color: lightblue;
    } 
</style> 
</head>  
<body>  
    <center> <h1> Add New Network </h1> </center> 
    <form action="/get">
        <div class="container"> 
            <label>Enter Router SSID : </label> 
            <input type="text" placeholder="Enter SSID" name="ssid" required>
            <label>Password : </label> 
            <input type="text" placeholder="Enter Password" name="password" required>
            <button type="submit">Add</button> 
        </div> 
    </form>   
</body>   
</html>
)rawliteral";
/*=============================================================================
 *                             Function Declarations
 *==============================================================================*/
/**
 * @brief initialize and start Web page
 *
 * @return nothing
 */

void web_init();
