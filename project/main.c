

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "leds.h"
#include "epuck1x/uart/e_uart_char.h"
#include "spi_comm.h"
#include "serial_comm.h"
#include "motors.h"
#include "sensors/proximity.h"

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


messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

int main(void)
{
	messagebus_init(&bus, &bus_lock, &bus_condvar);

  //  clear_leds();
   // spi_comm_start();
    halInit();
    chSysInit();
    mpu_init();

    serial_start();
    motors_init();
    proximity_start();
    calibrate_ir();

    //chThdSleepMilliseconds(1000);

    /* Infinite loop. */

    unsigned int fr = 0;
    unsigned int fl = 7;
    unsigned int rr = 1;
    unsigned int ll = 6;

    unsigned int obs = 0;
    unsigned int turntime = 0;

    while (1)
    {


    	if (obs == 0){
    		left_motor_set_speed(200);
    		right_motor_set_speed(200);

    		// sensors 0,1,6,7

    		if ((get_prox(fr) > 200) || (get_prox(fl) > 200) || (get_prox(rr) > 600) || (get_prox(ll) > 600)){
    			obs = 1;
    		}
    	//	if ((get_prox(fr) > 200) && (get_prox(fl) > 200)  (get_prox(rr) > 200) || (get_prox(ll) > 200)){
		//		obs = 2;
		//	}

    		chThdSleepMilliseconds(100);

    		char str[100];
			int str_length;
			str_length = sprintf(str, "obs: %d\n",obs);
			e_send_uart1_char(str, str_length);
    	}

    	if (obs == 1){

    		while ((get_prox(fr) > 200) || (get_prox(rr) > 600)){
    			left_motor_set_speed(-200);
    			right_motor_set_speed(200);
    			//obs = 1;
			}
    		while ((get_prox(fl) > 200) || (get_prox(ll) > 600)){
    			left_motor_set_speed(200);
    			right_motor_set_speed(-200);
    			//obs = 1;
			}
    		turntime = 500 +(rand() % 1000);
    		chThdSleepMilliseconds(turntime);

    		char str[100];
			int str_length;
			str_length = sprintf(str, "obs: %d\n",obs);
			e_send_uart1_char(str, str_length);

			str_length = sprintf(str, "turntime: %d\n",turntime);
			e_send_uart1_char(str, str_length);

			obs = 0;
    	}





    	/////////////////////////////
    	//WORKING CODE TO COPY FROM//
    	//unsigned int on = 1;
    	//unsigned int off = 0;
    	//set_body_led(on);
    	//left_motor_set_speed(500);
    	//right_motor_set_speed(-500);
    	//chThdSleepMilliseconds(1000);


    	//char str[100];
    	//int str_length;

    	//unsigned int sensor_number = 0;
    	//int value = get_prox(sensor_number);

    	//str_length = sprintf(str, "Sensing: %d!\n",value);
    	//e_send_uart1_char(str, str_length);

    	//set_body_led(off);
    	//left_motor_set_speed(-500);
    	//right_motor_set_speed(500);
    	//chThdSleepMilliseconds(200);
    	//////////////////////////////

    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
