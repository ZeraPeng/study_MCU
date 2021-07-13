// LPM_key_LED
#include "msp.h"

int main(void)
{
    // Hold the watchdog
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    P1DIR &= ~(BIT1+BIT4);
    P1REN |= (BIT1+BIT4);
    P1OUT |= (BIT1+BIT4);
    P2DIR |= (BIT0+BIT1+BIT2);
    P2OUT &= ~(BIT0+BIT1+BIT2);

    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1IES |= BIT1; // Interrupt on high-to-low transition 下降沿
    P1IES |= BIT4;
    P1IFG = 0; // Clear all P1 interrupt flags    中断标志
    P1IE = BIT1; // Enable interrupt for P1.1     中断使能
    P1IE |= BIT4;

    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    // Enable PCM rude mode, which allows to device to enter LPM3 without waiting for peripherals
    PCM->CTL1 = PCM_CTL1_KEY_VAL|PCM_CTL1_FORCE_LPM_ENTRY;

    __enable_interrupts();
    // Setting the sleep deep bit
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);
    // Do not wake up on exit from ISR
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    // Go to LPM4
    __sleep();
}

void PORT1_IRQHandler(void) {
    volatile uint32_t i;
    //char keycode;
    //keycode = Is_key_pressed();
    if(P1->IFG & BIT1){
        P2OUT |= BIT0;
        P2OUT &= ~BIT1;
    }
    if(P1->IFG & BIT4){
        P2OUT |= BIT1;
        P2OUT &= ~BIT0;
    }

    P1IFG = 0;
}

