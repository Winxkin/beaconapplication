#include <stdio.h> 
#include <stdint.h>
#include "app_led.h"
#include "nrf_gpio.h"
#include "cus_board.h"

void led_on (int pin) {
	nrf_gpio_pin_set(pin);
	
}

void led_off(int pin){
	nrf_gpio_pin_clear(pin);
	
}

void led_init (void) {
	nrf_gpio_cfg_output(LED1);
	nrf_gpio_cfg_output(LED2);	
}