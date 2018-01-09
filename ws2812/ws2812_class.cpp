/*
 * ws2812_class.cpp
 *
 *  Created on: 2017. nov. 12.
 *      Author: mester
 */

#include "ws2812_class.h"
extern "C" {
#include "ws2812.h"
}



ws2812::ws2812()
{
    uint16_t *buf;

    buf = fillBufferBlack();
    writedata(buf, LED_BUFFER_SIZE);
    //timer2.setTimeBaseByFrequency(10000000,49999);
    timer2.initialiseTimeBase(89, 0, 0,TIM_CounterMode_Up);
    //timer2.initCompareForPwmOutput(50, TIM_OCMode_PWM1, TIM_OCPolarity_High);
    timer2.initCompare(50, TIM_OCMode_PWM1, TIM_OCPolarity_High, TIM_OCPreload_Enable);
    timer2.enablePeripheral();


}

ws2812::~ws2812()
{

}

void ws2812::setcolor(uint32_t LEDnumber, uint8_t RED, uint8_t GREEN, uint8_t BLUE)
{
    uint16_t *buf;

    buf = setLEDcolor(LEDnumber, RED, GREEN, BLUE);

    writedata(buf, LED_BUFFER_SIZE);
}
void ws2812::writedata(uint16_t *data, uint32_t len)
{
    timerdma.beginWriteByTimer(timer2, data, len);
}
