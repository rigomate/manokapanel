/*
 * manotimer.cpp
 *
 *  Created on: 2018. jan. 8.
 *      Author: mester
 */




#include "manotimer.h"

extern "C"
{
    void SysTick_Handler_stm32plus(void)
    {
        MillisecondTimerMano::_counter++;
    }
}
