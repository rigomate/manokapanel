/*
 * stepper.h
 *
 *  Created on: 2017. okt. 14.
 *      Author: mester
 */

#include "config/stm32plus.h"
#include "config/spi.h"

using namespace stm32plus;

struct SpiPinSetup {
  enum {
    Port_NSS=GPIOA_BASE,
    Port_SCK=GPIOA_BASE,
    Port_MISO=GPIOA_BASE,
    Port_MOSI=GPIOA_BASE,

    Pin_NSS  = GPIO_Pin_4,
    Pin_SCK  = GPIO_Pin_5,
    Pin_MISO = GPIO_Pin_6,
    Pin_MOSI = GPIO_Pin_7
  };
};


class stepper_drv8806
{

public:
    //Constructor
    stepper_drv8806();

    //Destructor
    ~stepper_drv8806();

    void stepPositive(void);
    void stepNegative(void);

private:

    void init_driver(void);
    void StepperStep(uint8_t StepPos);

    uint8_t currentStepPos = 1;

    typedef Spi2<> MySender;
    MySender *sender;

    //GpioC<DefaultDigitalOutputFeature<10> > resetpin;
    GpioC<DefaultDigitalOutputFeature<13> > StepperEnablePin;
    GpioC<DefaultDigitalOutputFeature<15> > StepperResetPin;
    GpioB<DefaultDigitalOutputFeature<12> > StepperResetLatch;
};
