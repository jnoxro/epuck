#include "epuck1x/uart/e_uart_char.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



#include "leds.h"

#include "spi_comm.h"
#include "serial_comm.h"


#include "ch.h"
#include "hal.h"
#include "motors.h"
#include "memory_protection.h"
#include <main.h>

void clear_leds(void);
void spi_comm_start(void);
void motors_init(void);
void serial_start(void);
void e_send_uart1_char(const char * buff, int buff_len);

int main(void)
{

    halInit();
    chSysInit();
    mpu_init();


    /* Infinite loop. */
    while (1)
    {
    	unsigned int on = 1;
    	unsigned int off = 0;
    	void set_body_led(on);
    	chThdSleepMilliseconds(500);


    	charstr[100];
    	int str_length;
    	str_length = sprintf(str, "Hello World\n");
    	e_send_uart1_char(str, str_length);
    	//void set_led(LED1,0);
    	//chThdSleepMilliseconds(500);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
