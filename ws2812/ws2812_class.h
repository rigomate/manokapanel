/*
 * ws2812_class.h
 *
 *  Created on: 2017. nov. 12.
 *      Author: mester
 */

#include "config/stm32plus.h"
#include "config/dma.h"
#include "config/timing.h"

using namespace stm32plus;


class ws2812
{
public:
    //Constructor
    ws2812();

    //Destructor
    ~ws2812();
    void setcolor(uint32_t LEDnumber, uint8_t RED, uint8_t GREEN, uint8_t BLUE);


private:
    Dma1Channel7<
                    TimerDmaFeature<
                    Timer2Ccr4DmaPeripheralInfo,
                    TIM_DMA_CC4,
                    DMA_Priority_High,
                    DMA_Mode_Circular>
                  > timerdma;
    Timer2<
                   TimerChannel4Feature<>,
                   Timer2InternalClockFeature,
                   Timer2GpioFeature<TIMER_REMAP_FULL, TIM2_CH4_OUT>
                  > timer2;

    void writedata(uint8_t *data, uint32_t len);
    /*
    Dma1Channel7<
      TimerDmaFeature<
       Timer2Ccr4DmaPeripheralInfo,
       TIM_DMA_CC4,
       DMA_Priority_High,
       DMA_Mode_Circular>
    > *timerdma;

    Timer2<
      TimerChannel4Feature<>,
      Timer2InternalClockFeature,
      Timer2GpioFeature<TIMER_REMAP_FULL, TIM2_CH4_OUT>
    > *timer2;
    */
};
