// car_key
#include "msp.h"
#define LOAD P4OUT &= ~BIT6; for(i=500; i>0; i--); P4OUT |= BIT6;
#define CLK_0 P1OUT &= ~BIT5;
#define CLK_1 P1OUT |= BIT5;
#define MISO P1IN & BIT7
#define T 100000
/**
 * main.c
 */
void led2_r(void);
void led2_g(void);
void led2_b(void);
void led2_rg(void);
void led2_rb(void);
void led2_gb(void);
void led2_rgb(void);


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

    int i;
    // LED初始化
    P2DIR |= BIT0+BIT1+BIT2;

    // 初始化
    P4DIR |= BIT6;
    P4OUT |= BIT6;
    P1DIR &= ~BIT7;
    P1DIR |= BIT5;
    P1OUT &= ~BIT5;

    struct SENSORS sensor = {0};

    int flag;
    while(1)
    {
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

             // 红外
             sensor.ir_left_3 = !!(sensor.reg0&(1<<0));
             sensor.ir_left_2 = !!(sensor.reg0&(1<<1));
             sensor.ir_left_1 = !!(sensor.reg0&(1<<2));
             sensor.ir_mid = !!(sensor.reg0&(1<<3));
             sensor.ir_right_1 = !!(sensor.reg0&(1<<4));
             sensor.ir_right_2 = !!(sensor.reg0&(1<<5));
             sensor.ir_right_3 = !!(sensor.reg0&(1<<6));
             // 碰撞开关
             sensor.switcher_front_left_2 = (!(sensor.reg1&(1<<0)));   //左2
             sensor.switcher_front_left_1 = (!(sensor.reg1&(1<<1)));   //左1
             sensor.switcher_front_right_1 = (!(sensor.reg1&(1<<2)));
             sensor.switcher_front_right_2 = (!(sensor.reg1&(1<<3)));
             sensor.switcher_back_left = (!(sensor.reg1&(1<<4)));
             sensor.switcher_back_right = (!(sensor.reg1&(1<<5)));
             // 按键开关
             sensor.key_1 = (!(sensor.reg1&(1<<6)));
             sensor.key_2 = (!(sensor.reg1&(1<<7)));

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
}


