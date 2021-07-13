#include "msp.h"

int count = -1;
int debounce = 0;
int flag = 0;
int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_SSEL_1 | WDT_A_CTL_TMSEL | WDT_A_CTL_CNTCL | WDT_A_CTL_IS_5;

    // LED
    P2->DIR |= BIT0;
    P2->OUT &= ~BIT0;
    // S1
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    // S1_IRQ
    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1->IES |= BIT1;
    P1->IFG = 0;
    P1->IE = BIT1;
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    // Enable PCM rude mode, which allows to device to enter LPM3 without waiting for peripherals
    PCM->CTL1 = PCM_CTL1_KEY_VAL|PCM_CTL1_FORCE_LPM_ENTRY;
    __enable_interrupts();

    // WDT_A_IRQ
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    __DSB();
    __enable_irq();
    NVIC->ISER[0] = 1 << ((WDT_A_IRQn) & 31);

    while(1){
        __sleep();
    }
}

void PORT1_IRQHandler(void) {
    int i;
    for(i=2000; i>0; i--);
    if(P1->IFG & BIT1){
        if(count<0)
            P2OUT ^= BIT0;
        count++;
        count %= 3;
    }
    P1IFG = 0;
}

void WDT_A_IRQHandler(void)
{
    if(count < 0) return;
    if(flag == 0)
    {
        switch(count)
        {
            case 0: debounce = 4; break;
            case 1: debounce = 2; break;
            case 2: debounce = 1; break;
            default: break;
        }
        flag = 1;
    }

    if(debounce > 0)
    {
        debounce--;
        return;
    }
    else
    {
        P2->OUT ^= BIT0;
        flag = 0;
    }
}
