// car_key
#include "msp.h"
#define LOAD P4OUT &= ~BIT6; for(i=500; i>0; i--); P4OUT |= BIT6;
#define CLK_0 P1OUT &= ~BIT5;
#define CLK_1 P1OUT |= BIT5;
#define MISO P1IN & BIT7
/**
 * main.c
 */
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
    P1DIR |= BIT0;
    P1REN |= BIT0;
    P1OUT &= ~BIT0;

    //初始化
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

             sensor.switcher_front_left_2 = (!(sensor.reg1&(1<<0)));   //左2
             sensor.switcher_front_left_1 = (!(sensor.reg1&(1<<1)));   //左1
             sensor.switcher_front_right_1 = (!(sensor.reg1&(1<<2)));
             sensor.switcher_front_right_2 = (!(sensor.reg1&(1<<3)));
             sensor.switcher_back_left = (!(sensor.reg1&(1<<4)));
             sensor.switcher_back_right = (!(sensor.reg1&(1<<5)));
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


             sensor.switcher_front_left_2 = (!(sensor.reg1&(1<<0)));   //左2
             sensor.switcher_front_left_1 = (!(sensor.reg1&(1<<1)));   //左1
             sensor.switcher_front_right_1 = (!(sensor.reg1&(1<<2)));
             sensor.switcher_front_right_2 = (!(sensor.reg1&(1<<3)));
             sensor.switcher_back_left = (!(sensor.reg1&(1<<4)));
             sensor.switcher_back_right = (!(sensor.reg1&(1<<5)));

             sensor.key_1 = (!(sensor.reg1&(1<<6)));
             sensor.key_2 = (!(sensor.reg1&(1<<7)));

            flag = (sensor.switcher_back_left) || (sensor.switcher_front_left_2) || (sensor.switcher_front_left_1)
                    || (sensor.switcher_front_right_1) || (sensor.switcher_front_right_2) || (sensor.switcher_back_right);
            if(flag)
            {
                P1OUT |= BIT0;
                for(i=50000; i>0; i--);
                P1OUT &= ~BIT0;
            }
    }
}
