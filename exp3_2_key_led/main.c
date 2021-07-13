// key_led
#include "msp.h"

void delay_ms(int n);
unsigned char Is_key_pressed(void);

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    int keycode;
    P1DIR &= ~(BIT1+BIT4);
    P1REN |= (BIT1+BIT4);
    P1OUT |= (BIT1+BIT4);
    P2DIR |= (BIT0+BIT1+BIT2);
    P2OUT &= ~(BIT0+BIT1+BIT2);

	while(1)
	{
        keycode = Is_key_pressed();
	    switch(keycode)
	    {
	        case 1:
	        {
	            P2OUT |= BIT0;
	            P2OUT &= ~BIT1;
	            break;
	        }
	        case 2:
	        {
	            P2OUT |= BIT1;
                P2OUT &= ~BIT0;
	            break;
	        }
	        default: break;
	    }
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

void delay_ms(int n) {
    int i;
    for(i=n/0.00002; i>0; i--);
}
