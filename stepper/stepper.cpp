/*
 * stepper.cpp
 *
 *  Created on: 2017. okt. 14.
 *      Author: mester
 */


#include "stepper.h"

stepper_drv8806::stepper_drv8806()
{
    MySender::Parameters senderParams;
    senderParams.spi_mode=SPI_Mode_Master;

    sender = new MySender(senderParams);

    init_driver();
}

stepper_drv8806::~stepper_drv8806()
{
    //Todo destructing stuff
}

void stepper_drv8806::stepPositive()
{
    switch (currentStepPos) {
        case 1:
            currentStepPos = 2;
            break;
        case 3:
            currentStepPos = 2;
            break;
        case 2:
            currentStepPos = 4;
            break;
        case 6:
            currentStepPos = 4;
            break;
        case 4:
            currentStepPos = 8;
            break;
        case 12:
            currentStepPos = 8;
            break;
        case 8:
            currentStepPos = 1;
            break;
        default:
            break;
    }
    StepperStep(currentStepPos);
}

void stepper_drv8806::stepNegative()
{
    switch (currentStepPos) {
        case 8:
            currentStepPos = 4;
            break;
        case 12:
            currentStepPos = 4;
            break;
        case 4:
            currentStepPos = 2;
            break;
        case 6:
            currentStepPos = 2;
            break;
        case 2:
            currentStepPos = 1;
            break;
        case 3:
            currentStepPos = 1;
            break;
        case 1:
            currentStepPos = 8;
            break;
        default:
            break;
    }
    StepperStep(currentStepPos);
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
}

void stepper_drv8806::StepperStep(uint8_t StepPos)
{
    //MillisecondTimer::initialise();

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
