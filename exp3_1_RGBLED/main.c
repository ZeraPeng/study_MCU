// RGBLED
#include "msp.h"
#define T 25000

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	int i;
	P2DIR |= BIT0+BIT1+BIT2;

	while(1)
	{
	    P2OUT |= BIT0;
	    for(i=0; i<T; i++);
	    P2OUT &= ~BIT0;
	    P2OUT |= BIT1;
	    for(i=0; i<T; i++);
	    P2OUT &= ~BIT1;
	    P2OUT |= BIT2;
	    for(i=0; i<T; i++);
	    P2OUT &= ~BIT2;
	}
}
