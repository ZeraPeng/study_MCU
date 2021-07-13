// timer key lpm
#include "msp.h"
void cs_init(void);

int main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
    cs_init(); //clock system initial
    // Configure GPIO
    // LED2
    P2DIR |= (BIT0+BIT1+BIT2);
    P2OUT &= ~(BIT0+BIT1+BIT2);
    // S1
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCR[0] = 30000;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mod
                                    TIMER_A_CTL_MC__UP;
    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1IES |= BIT1; // Interrupt on high-to-low transition 下降沿
    P1IFG = 0; // Clear all P1 interrupt flags    中断标志
    P1IE = BIT1; // Enable interrupt for P1.1     中断使能

    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    __enable_irq(); // Enable global interrupt

    // Enable PCM rude mode, which allows to device to enter LPM3 without waiting for peripherals
    PCM->CTL1 = PCM_CTL1_KEY_VAL|PCM_CTL1_FORCE_LPM_ENTRY;
    // Setting the sleep deep bit
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);
    __enable_interrupts();
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // Enable sleep on exit from // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    while (1) {
        __sleep();
    }
}

// Timer A0 interrupt service routine
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    P2->OUT ^= BIT0;
}

void PORT1_IRQHandler(void) {
    int i;
    for(i=2000; i>0; i--);
    if(P1->IFG & BIT1){
        switch(TIMER_A0->CCR[0])
        {
            case 30000: TIMER_A0->CCR[0] = 15000; break;
            case 15000: TIMER_A0->CCR[0] = 7500; break;
            case 7500: TIMER_A0->CCR[0] = 30000; break;
        }
    }
    P1IFG = 0;
}

void cs_init(void) {
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
