

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "leds.h"
#include "epuck1x/uart/e_uart_char.h"
#include "spi_comm.h"
#include "serial_comm.h"
#include "motors.h"

#include "ch.h"
#include "hal.h"

#include "memory_protection.h"
#include <main.h>

//void set_body_led(unsigned int value);
//void clear_leds(void);
//void spi_comm_start(void);
//void motors_init(void);
//void serial_start(void);
//void e_send_uart1_char(const char * buff, int buff_len);



int main(void)
{
   // serial_start();

  //  clear_leds();
   // spi_comm_start();
    halInit();
    chSysInit();
    mpu_init();


    motors_init();
    //chThdSleepMilliseconds(1000);

    /* Infinite loop. */
    while (1)
    {
    	//unsigned int on = 1;
    	//unsigned int off = 0;
    	//set_body_led(on);
    	left_motor_set_speed(500);
    	right_motor_set_speed(-500);
    	chThdSleepMilliseconds(1000);


    	//char str[100];
    	//int str_length;

    	//str_length = sprintf(str, "Hello World\n");
    	//e_send_uart1_char(str, str_length);
    	//void set_led(LED1,0);
    	//chThdSleepMilliseconds(500);


    	//int value=10;
    	//str_length = sprintf(str, "Printing number %d!\n",value);
    	//e_send_uart1_char(str, str_length);

    	//set_body_led(off);
    	left_motor_set_speed(-500);
    	right_motor_set_speed(500);
    	chThdSleepMilliseconds(1000);

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
