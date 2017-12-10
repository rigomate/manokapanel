/*
 * pwm.cpp
 *
 *  Created on: 2017. dec. 2.
 *      Author: mester
 */


#include "pwm.h"

pwm::pwm()
{

    timer1_4.setTimeBaseByFrequency(100000, 255);
    timer1_4.initCompareForPwmOutput(100, TIM_OCMode_PWM1, TIM_OCPolarity_Low);
    timer1_4.enablePeripheral();
/*
    timer2_1.setTimeBaseByFrequency(100000, 255);
    timer2_1.initCompareForPwmOutput(0, TIM_OCMode_PWM1, TIM_OCPolarity_Low);
    timer2_1.enablePeripheral();
*/
    timer2_2.setTimeBaseByFrequency(100000, 255);
    timer2_2.initCompareForPwmOutput(0, TIM_OCMode_PWM1, TIM_OCPolarity_Low);
    timer2_2.enablePeripheral();

    timer3_2.setTimeBaseByFrequency(100000, 255);
    timer3_2.initCompareForPwmOutput(0, TIM_OCMode_PWM1, TIM_OCPolarity_Low);
    timer3_2.enablePeripheral();

    timer3_3.setTimeBaseByFrequency(100000, 255);
    timer3_3.initCompareForPwmOutput(100, TIM_OCMode_PWM1, TIM_OCPolarity_Low);
    timer3_3.enablePeripheral();

    timer3_4.setTimeBaseByFrequency(100000, 255);
    timer3_4.initCompareForPwmOutput(100, TIM_OCMode_PWM1, TIM_OCPolarity_Low);
    timer3_4.enablePeripheral();

}

void pwm::setpwm(uint8_t channel, uint8_t dutyratio)
{
    switch (channel) {
        case 14:
            timer1_4.setDutyCycle(dutyratio);
            break;
        case 21:
            //timer2_1.setDutyCycle(dutyratio);
            break;
        case 22:
            timer2_2.setDutyCycle(dutyratio);
            break;
        case 32:
            timer3_2.setDutyCycle(dutyratio);
            break;
        case 33:
            timer3_3.setDutyCycle(dutyratio);
            break;
        case 34:
            timer3_4.setDutyCycle(dutyratio);
            break;
        default:
            break;
    }

}
