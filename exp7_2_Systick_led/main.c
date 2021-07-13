// Systick_led
#include "msp.h"


/**
 * main.c
 */
int count = -1;
int debounce = 0;
int flag = 0;
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Configure GPIO
    // LED
    P2->DIR |= BIT0;
    P2->OUT &= ~BIT0;
    // S1
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    // Enable SysTick Module
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    // Set SysTick period = 0xB71B0, 4Hz/3MHz MCLK
    SysTick->LOAD = 0xB71B0 - 1;
    // Clear the SysTick current value register by writing
    SysTick->VAL = 0x01;
    // Enable SysTick interrupt
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    __enable_irq();

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

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    __DSB();

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

void SysTick_Handler(void)
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
