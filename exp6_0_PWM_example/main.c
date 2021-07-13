#include "msp.h"

void LeftMotor_Forward(int rate);
/**
 * main.c
 */
#include "msp.h"


/**
 * main.c
 */
void set_speed(int left,int right);
void LeftMotor_Forward(int left);
void LeftMotor_Back(int left);
void RightMotor_Forward(int right);
void RightMotor_Back(int right);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    //PxDS寄存器   输出驱动强度  BIT=1 高驱动
    P2->DS |= (BIT7+BIT4+BIT6);
    P2->DS |= BIT6;
    uint32_t k;
    //初始状态
    P1->DIR |= BIT0; // P1.0 set output
       P1->OUT &= ~BIT0;//turn off led
       P1->DIR &= ~BIT1;//setting IO for input
       P1->REN |=BIT1;// pull enable
       P1->OUT |=BIT1;//pull up
       while(1)
    {
    P1->OUT ^= BIT0;
    for(k=0;k<50000;k++);//Delay
    if ((P1->IN & BIT1) == 0)
    { P1->OUT &= ~BIT0;//turn off led
    break;}
    }
        //cs initial
        CS->KEY = 0x695A;
        CS->CTL1 = CS->CTL1&(~CS_CTL1_SELS_MASK)|CS_CTL1_SELS_0;
        CS->KEY = 0;

        //TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
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

           // 这一块儿要放到中断里
           int flag=1;
            if ((P1->IN & BIT1) == 0)
           {
                while(flag)
            {
            set_speed(512*0.4,512*0.4);
            for(k=0;k<500000;k++);//Delay
            flag=0;
            }
                flag=1;
                while(flag){
            set_speed(512*0.8,512*0.8);
            for(k=0;k<500000;k++);//Delay
            flag=0;
                }
                flag=1;
                while(flag){
            set_speed(-512*0.6,-512*0.6);
            for(k=0;k<500000;k++);//Delay
            flag=0;
                }
                set_speed(0,0);
           }

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;// Enable sleep on exit from ISR
    __DSB();
    __sleep();
}

void set_speed(int left,int right)
{
    //左轮：2.7 2.4 右轮：5.6，2.6
    P2->DIR |= (BIT4+BIT7+BIT6);
    P5->DIR |= (BIT6);
    P2->SEL1 &=~(BIT7+BIT4+BIT6);
    P5->SEL1 &=~BIT6;

    if(left>=0)
        LeftMotor_Forward(left);
    else
        LeftMotor_Back(left);
    if(right>=0)
        RightMotor_Forward(right);
    else
        RightMotor_Back(right);

}



void LeftMotor_Forward(int left)
{
//左轮 P2.7 and P2.4
            P2->OUT |= BIT7;
           P2->OUT &= ~BIT4;
           P2->SEL0 |= BIT7;
           P2->SEL0 &=~ BIT4;
           TIMER_A0->CCR[4] = left;
}
void LeftMotor_Back(int left)
{
    P2->OUT |= BIT4;
           P2->OUT &= ~BIT7;
           P2->SEL0 |= BIT4;
           P2->SEL0 &=~ BIT7;
           TIMER_A0->CCR[1] = -left;
}
void RightMotor_Forward(int right)
{
           P5->OUT |= BIT6;
           P2->OUT &= ~BIT6;
           P5->SEL0 |= BIT6;
           P2->SEL0 &=~ BIT6;
           TIMER_A2->CCR[1] = right;
}
void RightMotor_Back(int right)
{
        P2->OUT |= BIT6;
        P5->OUT &= ~BIT6;
        P2->SEL0 |= BIT6;
        P5->SEL0 &=~ BIT6;
        TIMER_A0->CCR[3] =-right;
}
