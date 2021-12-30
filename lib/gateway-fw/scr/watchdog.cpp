/**
 * @file watchdog.cpp
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Watchdog timer Code
 * @version 0.1
 * @date 30-12-2021
 * @copyright Copyright (c) 2021
 *
 */
/*=============================================================================
 *                                 Includes
 *==============================================================================*/
#include "config.h"
#include "watchdog.h"
#include <Arduino.h>
#include <stdio.h>

// Enable / Disable Watchdog timer. Comment for disable
#define WDT Y

/*=============================================================================
 *                                 Defines
 *==============================================================================*/
#define WDT_FEED_WORKER_DELAY_MS ((CONFIG_WATCHDOG_TIMEOUT_MSEC) / 2)

// Configure the Watchdog timer in Seconds
#define WDT_TIMEOUT 30

#define CPU_CORE 1

static TaskHandle_t WDT_Handler = NULL;

/*=============================================================================
 *                             Private Functions
 *==============================================================================*/
static void wdt_init_feed(void *parameters)
{
    int err;
    Serial.println("Configuring WDT...");
    err = esp_task_wdt_init(WDT_TIMEOUT, true);
    if (err != ESP_OK)
    {
        Serial.println("Error while Initializing WDT");
        return;
    }

    esp_task_wdt_add(NULL);

    while (1)
    {
        vTaskDelay(((WDT_TIMEOUT * 1000) / 2) / portTICK_PERIOD_MS);
        Serial.printf("Feed Watchdog timer for %d Seconds\n", WDT_TIMEOUT / 2);
        esp_task_wdt_reset();
    }
}

/*=============================================================================
 *                             Public Functions
 *==============================================================================*/
void watchdog_init_and_start()
{
    #ifdef WDT
    Serial.println("Starting the WDT Thread");
    delay(1000);
    xTaskCreatePinnedToCore(
        wdt_init_feed, // Function to trigger once the Task has been started
        "Task 2",      // name for the Task
        1024,          // Stack Size for the task stack
        NULL,          // Parameters to pass to function
        1,             // Task Priority 1 is lowest 25 is highest
        &WDT_Handler,  // Task Handler to manage this task from other task
        CPU_CORE);     // CPU core to RUN the code
    #endif
}

/*=============================================================================
 *                                End of File
 *==============================================================================*/