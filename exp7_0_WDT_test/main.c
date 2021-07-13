#include "msp.h"

int main(void){
    uint32_t i;
    // Configure the watchdog timer to timeout every ~175ms
    WDT_A -> CTL = WDT_A_CTL_PW | WDT_A_CTL_IS_3 | WDT_A_CTL_CNTCL;

    // Configure GPIO
    P1->DIR |= BIT0; // Set P1.0 to output - SET BREAKPOINT HERE
    P1->OUT ^= BIT0; // Toggle P1.0

    while(1)
    {
        for(i=2000; i>0; i--);
        WDT_A -> CTL = WDT_A_CTL_PW | WDT_A_CTL_IS_3 | WDT_A_CTL_CNTCL;
    }
}
