#include "msp.h"


/**
 * main.c
 */
void set_speed(int left, int right);
void left_forward(int left);
void left_back(int left);
void right_forward(int right);
void right_back(int right);
void cs_init(void);
void led_blink(void);

int k;
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// 输出设置
	P2->DS |= (BIT7+BIT4+BIT6);
	P2->DS |= BIT6;

    led_blink();

	// 时钟初始化
	cs_init();

	// 周期初始化
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP;
    TIMER_A0->CCR[0] = 512;
    TIMER_A2->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__UP;
    TIMER_A2->CCR[0] = 512;

    //左轮后退TA0.1--2.4
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[1] = 0;
    //右轮后退 TA0.3--2.6
    TIMER_A0->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[3] = 0;
    //左轮前进 TA0.4--2.7
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A0->CCR[4] = 0;
    //右轮前进TA2.1--5.6
    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7;
    TIMER_A2->CCR[1] = 0;

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
}

void set_speed(int left, int right){
    P2->DIR |= (BIT4+BIT7+BIT6);
    P5->DIR |= (BIT6);
    P2->SEL1 &=~(BIT7+BIT4+BIT6);
    P5->SEL1 &=~BIT6;

    if(left >= 0)
        left_forward(left);
    if(left < 0)
        left_back(-left);
    if(right >= 0)
        right_forward(right);
    if(right < 0)
        right_back(-right);
}

void left_forward(int left){
    P2->OUT |= BIT7;
    P2->OUT &= ~BIT4;
    P2->SEL0 |= BIT7;
    P2->SEL0 &=~ BIT4;
    TIMER_A0->CCR[4] = left;
}

void left_back(int left){
    P2->OUT |= BIT4;
    P2->OUT &= ~BIT7;
    P2->SEL0 |= BIT4;
    P2->SEL0 &=~ BIT7;
    TIMER_A0->CCR[1] = left;
}

void right_forward(int right){
    P5->OUT |= BIT6;
    P2->OUT &= ~BIT6;
    P5->SEL0 |= BIT6;
    P2->SEL0 &=~ BIT6;
    TIMER_A2->CCR[1] = right;
}

void right_back(int right){
    P2->OUT |= BIT6;
    P5->OUT &= ~BIT6;
    P2->SEL0 |= BIT6;
    P5->SEL0 &=~ BIT6;
    TIMER_A0->CCR[3] = right;
}

void cs_init(void){
    CS->KEY = 0x695A;
    CS->CTL1 = CS->CTL1&(~CS_CTL1_SELS_MASK)|CS_CTL1_SELS_0;
    CS->KEY = 0;
}

void PORT1_IRQHandler(void) {
    for(k=2000; k>0; k--);
    if ((P1->IN & BIT1) == 0)
    {
        set_speed(512*0.4,512*0.4);
        for(k=0;k<500000;k++);//Delay
        set_speed(0,0);
        set_speed(512*0.8,512*0.8);
        for(k=0;k<500000;k++);//Delay
        set_speed(0,0);
        set_speed(-512*0.6,-512*0.6);
        for(k=0;k<500000;k++);//Delay
        set_speed(0,0);
    }
    P1IFG = 0;
}

void led_blink(void){
    // LED 闪烁
    P1->DIR |= BIT0;
    P1->OUT &= ~BIT0;
    P1->REN |= BIT1;
    P1->OUT |= BIT1;
    while(1)
    {
        P1->OUT ^= BIT0;
        for(k=0;k<50000;k++);//Delay
        if ((P1->IN & BIT1) == 0)
        {
            P1->OUT &= ~BIT0;   //turn off led
            break;
        }
    }
}
