# include "msp.h"
void cs_init(void);
unsigned char Is_key_pressed(void);

int main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
    cs_init(); //clock system initial
    // Configure GPIO
    // LED2
    P2DIR |= (BIT0+BIT1+BIT2);
    P2OUT &= ~(BIT1+BIT2);
    P2OUT |= BIT0;
    // S1
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    // TA0CCR0为捕获/比较控制寄存器
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    // TA0CCR0为捕获/比较寄存器
    TIMER_A0->CCR[0] = 30000;
    // TA0CTL控制计数器部分
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK, continuous mod
                                    TIMER_A_CTL_MC__UP;
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31);
    __enable_irq(); // Enable global interrupt
    // __enable_interrupts();
    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // Enable sleep on exit from // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    while (1) {
        __sleep();
    }
}

// Timer A0 interrupt service routine
void TA0_0_IRQHandler(void) {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;  // CCTFG 各模块的捕获比较中断标志
    P1OUT ^= BIT0;
}

void PORT1_IRQHandler(void) {
    static int count = 0;
    char keycode;
    keycode = Is_key_pressed();
    if(P1IFG & BIT1){
        count++;
        count %= 3;
    }
    if(count == 0){
        TIMER_A0->CCR[0] = 25000;
    }
    if(count == 1){
        TIMER_A0->CCR[0] = 12500;
    }
    if(count == 2){
        TIMER_A0->CCR[0] = 6250;
    }
    P1IFG = 0;
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
