/*
 * potmeter.h
 *
 *  Created on: 2017. nov. 18.
 *      Author: mester
 */

#include "config/stm32plus.h"
#include "config/adc.h"
#include "config/dma.h"

#include <stdbool.h>
#include <vector>

using namespace stm32plus;

class potmeter{

public:

    potmeter(void);
    void startConversion(void);
    uint16_t getVal(uint8_t potNum);
private:

    void onComplete(DmaEventType det);
    uint32_t approxRollingAverage(float avg, uint32_t new_sample);

    Adc1DmaChannel<AdcDmaFeature<Adc1PeripheralTraits>,Adc1DmaChannelInterruptFeature> dma;
    Adc1<
      AdcClockPrescalerFeature<6>,                // PCLK2/6
      Adc1Cycle7RegularChannelFeature<0,1,2,3,4,5,6>,       // using channels 0,1 on ADC1 with 7-cycle latency
      AdcScanModeFeature                          // scan mode feature
    > adc;

    volatile bool _ready = false;

    //std::vector<uint16_t> readBuffer(7);
    //uint16_t readBuffer[7];
    //size_t size = 10;
    std::vector<uint16_t> readBuffer;
};
