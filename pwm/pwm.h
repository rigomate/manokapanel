/*
 * pwm.h
 *
 *  Created on: 2017. dec. 2.
 *      Author: mester
 */

#include "config/stm32plus.h"
#include "config/timing.h"

using namespace stm32plus;

class pwm
{

public:
    pwm();
    void setpwm(uint8_t channel, uint8_t dutyratio);

    Timer3<TimerChannel2Feature<>,
           //TimerChannel3Feature<>,
           //TimerChannel4Feature<>,
           Timer3InternalClockFeature,
           Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH2_OUT>
           //Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH3_OUT>,
           //Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH4_OUT>
    >timer3_2;

    Timer3<TimerChannel3Feature<>,
           //TimerChannel4Feature<>,
           Timer3InternalClockFeature,
           Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH3_OUT>
           //Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH4_OUT>
    >timer3_3;

    Timer3<TimerChannel4Feature<>,
           Timer3InternalClockFeature,
           Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH4_OUT>
    >timer3_4;

    Timer1<TimerChannel4Feature<>,
           Timer1InternalClockFeature,
           Timer1GpioFeature<TIMER_REMAP_NONE, TIM1_CH4_OUT>
    >timer1_4;
/*
    Timer2<TimerChannel1Feature<>,
           Timer2InternalClockFeature,
           Timer2GpioFeature<TIMER_REMAP_FULL, TIM2_CH1_OUT>
    >timer2_1;
*/
    Timer2<TimerChannel2Feature<>,
           Timer2InternalClockFeature,
           Timer2GpioFeature<TIMER_REMAP_FULL, TIM2_CH2_OUT>
    >timer2_2;

};
