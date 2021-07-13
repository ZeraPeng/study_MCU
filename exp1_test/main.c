#include "msp.h"

void main(void)
{
    volatile uint32_t i;

    WDTCTL = WDTPW | WDTHOLD;                    /* Stop watchdog timer */
    P1DIR |= BIT0;                               /* Configure P1.0 as output */

    while(1)
    {
        for(i=10; i>0; i--);                  /* Delay */
        P1OUT ^= BIT0;
    }
    // The following code toggles P1.0 port
                         /* Toggle P1.0 */
}
