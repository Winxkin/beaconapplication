/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup blinky_example_main main.c
 * @{
 * @ingroup blinky_example
 * @brief Blinky Example Application main file.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_led.h"
#include "cus_board.h"



/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Configure LED-pins as outputs.
		_init_();

    // Toggle LEDs.
    while (true)
    {
						led_on (LED);
						nrf_delay_ms (1000);	
						led_off(LED);				
            nrf_delay_ms (1000);
        
    }
}


/** @} */
