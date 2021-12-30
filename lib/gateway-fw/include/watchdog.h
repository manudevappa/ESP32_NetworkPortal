/**
 * @file watchdog.h
 * @author Manu Devappa (dmanu.techie@gmail.com)
 * @brief Source code for watchdog timer
 * @version 0.1
 * @date 30-12-2021
 *
 * @copyright Copyright (c) 2021
 *
 */


#ifndef WATCHDOG_H
#define WATCHDOG_H

/*=============================================================================
 *                                 Includes
 *==============================================================================*/

#include "deviceCommon.h"
#include <esp_task_wdt.h>

/*=============================================================================
 *                             Static Variables
 *==============================================================================*/

/*=============================================================================
 *                             Function Declarations
 *==============================================================================*/
/**
 * @brief initialize and start watchdog timer
 *
 * @return int
 */
void watchdog_init_and_start();
/*=============================================================================
 *                                End of File
 *==============================================================================*/



#endif