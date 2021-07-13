// car_key
#include "msp.h"
#include "comm.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P1DIR |= BIT0;
	P1REN |= BIT0;
	P1OUT &= ~BIT0;
	shift_reg_init();

	int flag;
	int i;
	while(1)
	{
	    flag = (sensor.switcher_back_left) || (sensor.switcher_front_left_2) || (sensor.switcher_front_left_1)
	                || (sensor.switcher_front_right_1) || (sensor.switcher_front_right_2) || (sensor.switcher_back_right);
	        if(flag)
	        {
	            P1OUT |= BIT0;
	            for(i=20000; i>0; i--);
	            P1OUT &= ~BIT0;
	        }
	}
}
