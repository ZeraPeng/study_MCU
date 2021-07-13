#include "msp.h"


/**
 * main.c
 */
void main(void)
{
	 WDTCTL = WDTPW + WDTHOLD;    // Stop WDT
	 P1DIR |= 0x01;
	 int i;
	 P1OUT &= ~0x01;
	 while(1)
	 {
	     P1OUT ^= 0x01;
	     for(i=1000000;i>0;i-=5);
	 }
}
