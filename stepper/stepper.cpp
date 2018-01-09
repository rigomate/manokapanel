/*
 * stepper.cpp
 *
 *  Created on: 2017. okt. 14.
 *      Author: mester
 */


#include "stepper.h"

stepper_drv8806::stepper_drv8806(uint32_t maxruntimems)
{
    MySender::Parameters senderParams;
    senderParams.spi_mode=SPI_Mode_Master;

    sender = new MySender(senderParams);

    MaxRunTimeMs = maxruntimems;
    //for overheat protection
    MillisecondTimerMano::initialise();

    overheatprotectionactive = false;
    isEnabled = false;
    init_driver();
}

stepper_drv8806::~stepper_drv8806()
{
    //Todo destructing stuff
}

void stepper_drv8806::stepPositive()
{
    currentStepPos++;
    if (currentStepPos > 7)
        {
            currentStepPos = 0;
        }
    StepperStep(StepTable[currentStepPos]);
}

void stepper_drv8806::operator++()
        {
            stepPositive();
        }

void stepper_drv8806::stepNegative()
{
    currentStepPos--;
    if (currentStepPos < 0)
        {
            currentStepPos = 7;
        }
    StepperStep(StepTable[currentStepPos]);
}

void stepper_drv8806::operator--()
        {
            stepNegative();
        }


void stepper_drv8806::init_driver()
{
    StepperEnablePin[13].set();

    StepperResetPin[15].set();
    StepperResetPin[15].reset();

    StepperEnablePin[13].reset();

    //latch to low
    StepperResetLatch[12].reset();

    StepperStep(currentStepPos);
    Enable();
}

void stepper_drv8806::StepperStep(uint8_t StepPos)
{


    //Send Data
    sender->send(StepPos);

    while (!sender->readyToSend());

    for (int i = 0; i< 500; i++)
        {
            __NOP();
        }

    //MillisecondTimer::delay(1);
    //Do a Latch
    StepperResetLatch[12].set();
    StepperResetLatch[12].reset();
}

void stepper_drv8806::Disable(void)
{
    if (isEnabled)
        {
            StepperEnablePin[13].set();
            isEnabled = false;
            overheatprotectionactive = false;
        }
}

void stepper_drv8806::Enable(void)
{
    static uint32_t now;

    if (!isEnabled)
        {
            now = MillisecondTimerMano::millis();
            StepperEnablePin[13].reset();
            isEnabled = true;
        }
    if (MillisecondTimerMano::hasTimedOut(now, MaxRunTimeMs))
    {
        overheatprotectionactive = true;
    }
    if (overheatprotectionactive)
    {
        StepperEnablePin[13].set();
    }
}
