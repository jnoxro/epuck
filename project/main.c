

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

    unsigned int task = 2;

    // TASK 1 SETTINGS
    unsigned int fr1 = 0;
    unsigned int fl1= 7;
    unsigned int rr1 = 1;
    unsigned int ll1 = 6;

    unsigned int obs = 0;
    unsigned int turntime = 0;

    unsigned int speed = 300;


    // TASK 2 SETTINGS

	 unsigned int fr = 0;
	 unsigned int fl = 7;
	 unsigned int frr = 1;
	 unsigned int fll = 6;
	 unsigned int rr = 2;
	 unsigned int ll = 5;
	 unsigned int bl = 4;
	 unsigned int br = 3;

	 // int a = 3000;
	  int far = 70;
	  int close = 1000;


    while (1)
    {



    	if (task == 1){
			if (obs == 0){
				left_motor_set_speed(speed);
				right_motor_set_speed(speed);

				// sensors 0,1,6,7

				if ((get_prox(fr1) > 200) || (get_prox(fl1) > 200) || (get_prox(rr1) > 600) || (get_prox(ll1) > 600)){
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


				if (((get_prox(fr1) > 200) && (get_prox(fl1) > 200)) || ((get_prox(ll1) > 200) && (get_prox(rr1) > 200))){
					turntime = 50;
				}
				else {
					turntime = 500 +(rand() % 750);
				}

				while ((get_prox(fr1) > 200) || (get_prox(rr1) > 600)){
					left_motor_set_speed(-speed);
					right_motor_set_speed(speed);
					//obs = 1;
				}
				while ((get_prox(fl1) > 200) || (get_prox(ll1) > 600)){
					left_motor_set_speed(speed);
					right_motor_set_speed(-speed);
					//obs = 1;
				}

				chThdSleepMilliseconds(turntime);

				char str[100];
				int str_length;
				str_length = sprintf(str, "obs: %d\n",obs);
				e_send_uart1_char(str, str_length);

				str_length = sprintf(str, "turntime: %d\n",turntime);
				e_send_uart1_char(str, str_length);

				obs = 0;
			}
    	}

    	if (task == 2){
    		char str[100];
    		int str_length;

    		//int far = 70;
    		//int close = 1000;


    		 if((get_prox(fr) < close) && (get_prox(fr) > far) && (get_prox(fl) < close) && (get_prox(fl) > far)){
				//If prox ranges for both sensors fall in set range between far and close, move forward
				str_length = sprintf(str, "1");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(200);
				right_motor_set_speed(200);
    		 }
			 if((get_prox(fr) < close) && (get_prox(fr) > far) && (get_prox(fl) < far)){
				//If prox range for fr is fine but fl value is too large meaning object too far away, speed up left
				str_length = sprintf(str, "2");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(400);
				right_motor_set_speed(200);
			 }
			 if((get_prox(fl) < close) && (get_prox(fl) > far) && (get_prox(fr) < far)){
				//If prox range for fl is fine but fr value is too large meaning object too far away, speed up right
				str_length = sprintf(str, "3");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(200);
				right_motor_set_speed(400);
			 }
			 if((get_prox(fr) < close) && (get_prox(fr) > far) && (get_prox(fl) > close)){
				//If prox range for fr is fine but fl value is too small meaning object too far, slow down left
				str_length = sprintf(str, "4");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(100);
				right_motor_set_speed(200);
			 }
			 if((get_prox(fl) < close) && (get_prox(fl) > far) && (get_prox(fr) > close)){
				//If prox range for fl is fine but fr value is too small meaning object too far, slow down right
				str_length = sprintf(str, "5");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(200);
				right_motor_set_speed(100);
			 }
    		 if((get_prox(fl) < far) && (get_prox(fr) < far) && ((get_prox(frr) > far) || (get_prox(rr) > far) || (get_prox(br) > far))){
				//if the right/back right sensors detect, then spin clockwise
				str_length = sprintf(str, "6");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(200);
				right_motor_set_speed(-200);
    		 }
    		 if((get_prox(fl) < far) && (get_prox(fr) < far) && ((get_prox(fll) > far) || (get_prox(ll) > far) || (get_prox(bl) > far))){
				//if the left/back left sensors detect, then spin counter clockwise
				str_length = sprintf(str, "7");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(-200);
				right_motor_set_speed(200);
    		 }
    		 if((get_prox(fr) > close) && (get_prox(fl) > close)){
				//If fron prox both too far, reverse robot
				str_length = sprintf(str, "8");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(-200);
				right_motor_set_speed(-200);
    		 }
/*    		 if((get_prox(fl) < close) && (get_prox(fl) > (2*far)) && (get_prox(fr) < close) && (get_prox(fr) > (2*far))){
				//If prox ranges for both sensors fall in set range between far and close, robot stops
				str_length = sprintf(str, "9");
				e_send_uart1_char(str, str_length);

				left_motor_set_speed(0);
				right_motor_set_speed(0);
    		 }*/
    	}
    	if (task == 3){ //test code
    		char str[100];
    		int str_length;
    		int value = get_prox(fl);
    		str_length = sprintf(str, "FL: %d\n",value);
    		e_send_uart1_char(str, str_length);

    		value = get_ambient_light(fl);
    		str_length = sprintf(str, "AMB: %d\n",value);
    		e_send_uart1_char(str, str_length);
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
