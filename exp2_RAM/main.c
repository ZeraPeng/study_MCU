#include <msp.h>
#include "stdint.h"
#define NumToWrite 128

void main(void) {
    uint8_t i;
    uint32_t j;
    uint8_t *RAM_ptr = (uint8_t *) 0x01000080;
    WDTCTL = WDTPW + WDTHOLD;    // Stop WDT
    P1DIR |= BIT0;
    for(i=0; i<NumToWrite; i++)
    {
        *RAM_ptr++ = i;
    }
    RAM_ptr = (uint8_t *) 0x01000080;
    for(i=0; i<NumToWrite; i++)
    {
        if (*RAM_ptr!=i) break;
        else RAM_ptr++;
    }
    while(i==NumToWrite)
    {
        P1->OUT ^= BIT0;
        for(j=0;j<50000;j++);   //Delay
    }
    while(i<NumToWrite)
    {
        P1OUT = BIT0;
    }
    for(j=0;j<50000;j++);   //Delay
}

