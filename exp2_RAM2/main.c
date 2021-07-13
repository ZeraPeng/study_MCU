#include <msp.h>
#include "stdint.h"

void main(void) {
    uint8_t i;
    uint32_t j;
    char *RAM_ptr = (char *) 0x20000000;
    char *RAM_ptr_2 = (char *) 0x20001000;
    WDTCTL = WDTPW + WDTHOLD;    // Stop WDT
    P1DIR |= BIT0;

    char data[] = "Hi£¬micro computer! Enjoy MCU!";
    i = 0;
    while(data[i] != '\0')
    {
        i++;
    }
    uint8_t len = i + 1;
    for(i=0; i<len; i++)
    {
        *RAM_ptr++ = data[i];
    }
    RAM_ptr = (char *) 0x20000000;
    for(i=0; i<len; i++)
    {
        *RAM_ptr_2++ = data[i];
    }
    RAM_ptr_2 = (char *) 0x20001000;
    for(i=0; i<len; i++)
    {
        if (*RAM_ptr != *RAM_ptr_2)
            break;
        else
            {
                RAM_ptr++;
                RAM_ptr_2++;
            }
    }
    while(i==len)
    {
        P1->OUT ^= BIT0;
        for(j=0;j<50000;j++);   //Delay
    }
    while(i<len)
    {
        P1OUT = BIT0;
    }
    for(j=0;j<50000;j++);   //Delay
}

