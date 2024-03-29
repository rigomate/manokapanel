/*
 * potmter.cpp
 *
 *  Created on: 2017. nov. 18.
 *      Author: mester
 */

#include "potmeter.h"

volatile int debug;
potmeter::potmeter(void)
{
    readBuffer.resize(7);

    dma.DmaInterruptEventSender.insertSubscriber(
        DmaInterruptEventSourceSlot::bind(this,&potmeter::onComplete)
    );
    dma.enableInterrupts(Adc1DmaChannelInterruptFeature::COMPLETE);
    dma.beginRead(&readBuffer[0],readBuffer.size());
    adc.startRegularConversion();
}

void potmeter::startConversion(void)
{
    adc.startRegularConversion();
}

uint16_t potmeter::getVal(uint8_t potNum)
{
    static float average[7] = {0};
    average[potNum] = approxRollingAverage(average[potNum], readBuffer[potNum]);
    return (uint16_t)average[potNum];
    //return readBuffer[potNum];
}

void potmeter::onComplete(DmaEventType det) {
  if(det==DmaEventType::EVENT_COMPLETE)
    _ready=true;
}

uint32_t potmeter::approxRollingAverage(float avg, uint32_t new_sample)
{
    avg -= avg / 10;
    avg += new_sample / 10;

    return avg;
}
