/*
 * comm.h
 * 传感器
 *  Created on: 2021年7月7日
 *      Author: penghan
 */

#ifndef COMM_H_
#define COMM_H_

#include "msp.h"

#define LOAD P4OUT &= ~BIT6; for(i=500; i>0; i--); P4OUT |= BIT6;
#define CLK_0 P1OUT &= ~BIT5;
#define CLK_1 P1OUT |= BIT5;
#define MISO P1IN & BIT7

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
extern struct SENSORS sensor;

void reload_shift_reg(void);
void shift_reg_init(void);

#endif /* COMM_H_ */

