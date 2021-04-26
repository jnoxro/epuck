#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "leds.h"
#include "spi_comm.h"
#include "selector.h"
#include "motors.h"

void motors_init(void);
void clear_leds(void);
void spi_comm_start(void);


int main(void)
{
	// To add to Task 1 (and 2):
	// Ambient light compensation

	// For Task 2:
	// Robot can approach the red object - red object is not far away and is the only nearby object
	// Red object detected using proximity sensors alone - detect from any direction and/or move in any direction?
	// Robot is able to chase red object!
	// Robot does NOT collide with red object - robot retreats if object approaches it to prevent collision!

	// Logic:
	// if(front two prox sensors are above certain value and equal, move forward)
	// if(FL sensor has higher or lower value (is closer to object), speed up right to turn left
	// if(FR sensor has higher/lower value (is closer to object), speed up left to turn right
	// if(FL and FR sensor nul (nothing detected), rotate on spot until both are detecting, then do logic above
	// 		if(sensors Ps1,2 or 3 detecting, rotate on spot ClockWise)
	// 		if(sensors Ps4,5 or 6 detecting, rotate on spot Counter ClockWise)
	// if(FL and FR are above certain value (object too close) full reverse
	// if(FL and FR equal certain value/range of values, full robot stop

	// Notes:
	// For prx sensors, close is small number, far away is large
	// ~10cm =  ~3500
	// ~1.5cm = ~1500

	// http://www.e-puck.org/index.php?option=com_content&view=article&id=22&Itemid=13

	//							fl = 7			 fr = 0
	//
	//
	//
	// 				fll = 6									frr = 1
	//
	//
	//
	//
	//
	//	 ll = 5 															rr = 2
	//
	//
	//
	//
	//
	// 				bl = 4									br = 3
	//
	//
	// x = 2000			Variable to change depending upon prox sensor requirements
	// y = 1500			Variable to change depending upon prox sensor requirements
	// z = 500			Variable to change depending upon prox sensor requirements


	// if((get_prox(fr) > y) && (get_prox(fr) < x) && (get_prox(fl) > y) && (get_prox(fl) < x))
	//		If prox ranges for both sensors fall in set range between x and y, move forward
	//		left_motor_set_speed(200);
	//		right_motor_set_speed(200);
	//
	// if((get_prox(fr) > y) && (get_prox(fr) < x) && (get_prox(fl) > x))
	//		If prox range for fr is fine but fl value is too large meaning object too far away, speed up left
	//		left_motor_set_speed(400);
	//		right_motor_set_speed(200);
	//
	// if((get_prox(fl) > y) && (get_prox(fl) < x) && (get_prox(fr) > x))
	//		If prox range for fl is fine but fr value is too large meaning object too far away, speed up right
	//		left_motor_set_speed(200);
	//		right_motor_set_speed(400);
	//
	// if((get_prox(fr) > y) && (get_prox(fr) < x) && (get_prox(fl) < y))
	//		If prox range for fr is fine but fl value is too small meaning object too close, slow down left
	//		left_motor_set_speed(100);
	//		right_motor_set_speed(200);
	//
	// if((get_prox(fl) > y) && (get_prox(fl) < x) && (get_prox(fr) < y))
	//		If prox range for fl is fine but fr value is too small meaning object too close, slow down right
	//		left_motor_set_speed(200);
	//		right_motor_set_speed(100);



























    halInit();
    chSysInit();
    mpu_init();

    int red_val = 0;
    int green_val = 0;
    int blue_val = 0;

    // void set_rgb_led(rgb_led_name_t led_number, int red_val, int green_val, int  blue_val);		// LED2,4,6, or 8

    /* Infinite loop. */
    while (1) {
    	set_rgb_led(LED2,0,1,0);		//Set RGB LED2 to Green
    	//waits 1 second
        chThdSleepMilliseconds(1000);	//Waits 1 second
        set_rgb_led(LED2,0,0,0);		//Set RGB LED2 to off
    }

    while (1){
    	if(get_selector() == 7){
    		left_motor_set_speed(500);
    		right_motor_set_speed(-500);
    	} else if(get_selector() == 13){
    		left_motor_set_speed(-500);
    		right_motor_set_speed(500);
    	}


    }
}



#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
