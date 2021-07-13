#include "msp.h"


/**
 * main.c
 */
void cs_init(void);
unsigned char Is_key_pressed(void);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	int i;
    int keycode;
    cs_init();
    P1DIR &= ~(BIT1+BIT4);
    P1REN |= (BIT1+BIT4);
    P1OUT |= (BIT1+BIT4);
	P2DIR |= BIT0;
	P2OUT &= ~BIT0;

	while(1)
	{
	    keycode = Is_key_pressed();
	    switch(keycode)
	    {
	          case 1:
	          {
	                CS->KEY = 0x695A;
	                CS->CTL1 = CS->CTL1&(~CS_CTL1_DIVM_MASK)|CS_CTL1_DIVM_3;
	                CS->KEY = 0;
	                break;
	           }
	           case 2:
	           {
	                CS->KEY = 0x695A;
	                CS->CTL1 = CS->CTL1&(~CS_CTL1_DIVM_MASK)|CS_CTL1_DIVM_2;
	                CS->KEY = 0;
	                break;
	           }
	           default: break;
	    }
	    P2OUT |= BIT0;
	    for(i=80000;i>0;i-=5);
	    P2OUT &= ~BIT0;
	    for(i=300000;i>0;i-=5);
	}
}

unsigned char Is_key_pressed(void) {
      int i;
      if ((P1IN & (BIT1+BIT4))==(BIT1+BIT2))
         return 0;            //no key pressed}
      else
      {
           for(i=2000; i>0; i--);
           if ((P1IN & BIT1) == 0) { return 1; }
           if ((P1IN & BIT4) == 0) { return 2; }
           return 0;
      }
}

void cs_init(void)
{
    // unlock CS module for register access
    CS->KEY = 0x695A;
    // set MCLK source from hfxtclk
    CS->CTL1 = CS->CTL1&(~CS_CTL1_SELM_MASK)|CS_CTL1_SELM_5;
    // set MCLK/16
    CS->CTL1 = CS->CTL1&(~CS_CTL1_DIVM_MASK)|CS_CTL1_DIVM_4;
    // set ACLK source from LFXTCLK
    CS->CTL1 = CS->CTL1&(~CS_CTL1_SELA_MASK)|CS_CTL1_SELA_0;
    // set SMCLK source from REFOCLK
    CS->CTL1 = CS->CTL1&(~CS_CTL1_SELS_MASK)|CS_CTL1_SELS_2;
    // set HFXTFREQ frequency range
    CS->CTL2 = CS->CTL2&(CS_CTL2_HFXTFREQ_MASK)|CS_CTL2_HFXTFREQ_6;
    // lock CS module from unintended access
    CS->KEY = 0;
}
