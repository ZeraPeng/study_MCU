// car_LPM_key_ir
#include "msp.h"
#define LOAD P4OUT &= ~BIT6; for(i=500; i>0; i--); P4OUT |= BIT6;
#define CLK_0 P1OUT &= ~BIT5;
#define CLK_1 P1OUT |= BIT5;
#define MISO P1IN & BIT7
#define T 100000
/**
 * main.c
 */

unsigned char Is_key_pressed(char keycode);

    struct SENSORS
    {
        //移位寄存器读取值
        unsigned char reg0;
        unsigned char reg1;

        //下方红外传感器
        //                         ir_mid
        //                ir_left_1      ir_right_1
        //        ir_left_2                       ir_right_2
        //ir_left_3                                        ir_right_3
        char ir_left_1;
        char ir_left_2;
        char ir_left_3;
        char ir_mid;
        char ir_right_1;
        char ir_right_2;
        char ir_right_3;
        //周围碰撞开关
        //        switcher_front_left1    switcher_front_right1
        //switcher_front_left2                    switcher_front_right2
        //
        //
        //    switcher_back_left              switcher_back_right
        char switcher_front_left_1;
        char switcher_front_left_2;
        char switcher_front_right_1;
        char switcher_front_right_2;
        char switcher_back_left;
        char switcher_back_right;
        //板载按键
        char key_1;
        char key_2;
    };

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // LED初始化
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P2DIR |= BIT0+BIT1+BIT2;
    P2OUT &= ~(BIT0+BIT1+BIT2);

    // 小车初始化
    P4DIR |= BIT6;
    P4OUT |= BIT6;
    P1DIR &= ~BIT7;
    P1DIR |= BIT5;
    P1OUT &= ~BIT5;

    // 按键初始化
    P1DIR &= ~(BIT1+BIT4);
    P1REN |= (BIT1+BIT4);
    P1OUT |= (BIT1+BIT4);

    P1->SEL0 = 0;
    P1->SEL1 = 0;
    P1IES |= BIT1; // Interrupt on high-to-low transition 下降沿
    P1IES |= BIT4;
    P1IFG = 0; // Clear all P1 interrupt flags    中断标志
    P1IE |= BIT1; // Enable interrupt for P1.1     中断使能
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
    struct SENSORS sensor = {0};

    // 刷新传感器
    CLK_0;
    LOAD;
    sensor.reg0 = 0;
    sensor.reg1 = 0;

    for (i = 0; i < 8; i++)
    {
        sensor.reg0 <<= 1;
        if (MISO)
            sensor.reg0 |= 0x01;
        CLK_1;
        CLK_0;
    }
    for (i = 0; i < 8; i++)
    {
        sensor.reg1 <<= 1;
        if (MISO)
            sensor.reg1 |= 0x01;
        CLK_1;
        CLK_0;
    }

    if(P1->IFG & BIT1){
        // 读取红外对管
        sensor.ir_left_3 = !!(sensor.reg0&(1<<0));
        sensor.ir_left_2 = !!(sensor.reg0&(1<<1));
        sensor.ir_left_1 = !!(sensor.reg0&(1<<2));
        sensor.ir_mid = !!(sensor.reg0&(1<<3));
        sensor.ir_right_1 = !!(sensor.reg0&(1<<4));
        sensor.ir_right_2 = !!(sensor.reg0&(1<<5));
        sensor.ir_right_3 = !!(sensor.reg0&(1<<6));
        // LED亮灯
        if(sensor.ir_left_3)
        {
            P2OUT |= BIT0+BIT1;
            for(i=0; i<T; i++);
            P2OUT &= ~(BIT0+BIT1);
        }
        if(sensor.ir_left_2) {
            P2OUT |= BIT1;
            for(i=0; i<T; i++);
            P2OUT &= ~BIT1;
        }
        if(sensor.ir_left_1){
            P2OUT |= BIT0;
            for(i=0; i<T; i++);
            P2OUT &= ~BIT0;
        }
        if(sensor.ir_right_1) {
            P2OUT |= BIT0+BIT1+BIT2;
            for(i=0; i<T; i++);
            P2OUT &= ~(BIT0+BIT1+BIT2);
        }
        if(sensor.ir_right_2) {
            P2OUT |= BIT1+BIT2;
            for(i=0; i<T; i++);
            P2OUT &= ~(BIT1+BIT2);
        }
        if(sensor.ir_right_3) {
            P2OUT |= BIT0+BIT2;
            for(i=0; i<T; i++);
            P2OUT &= ~(BIT0+BIT2);
        }
        if(sensor.ir_mid){
            P2OUT |= BIT2;
            for(i=0; i<T; i++);
            P2OUT &= ~BIT2;
        }

    }

    if(P1->IFG & BIT4){
        // 读取碰撞开关
        sensor.switcher_front_left_2 = (!(sensor.reg1&(1<<0)));   //左2
        sensor.switcher_front_left_1 = (!(sensor.reg1&(1<<1)));   //左1
        sensor.switcher_front_right_1 = (!(sensor.reg1&(1<<2)));
        sensor.switcher_front_right_2 = (!(sensor.reg1&(1<<3)));
        sensor.switcher_back_left = (!(sensor.reg1&(1<<4)));
        sensor.switcher_back_right = (!(sensor.reg1&(1<<5)));

        if((sensor.switcher_back_left) || (sensor.switcher_front_left_2) || (sensor.switcher_front_left_1)|| (sensor.switcher_front_right_1) || (sensor.switcher_front_right_2) || (sensor.switcher_back_right))
        {
            P1OUT |= BIT0;
            for(i=50000; i>0; i--);
            P1OUT &= ~BIT0;
        }
    }
    P1->IFG &=~(BIT1+BIT4);
}


