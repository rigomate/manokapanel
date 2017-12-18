/*
 * buttonpoti.h
 *
 *  Created on: 2017. dec. 16.
 *      Author: mester
 */

#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/button.h"
#include <stdbool.h>


typedef enum{
    button_inavtive,
    button_active,
    button_down,
    button_up,
}buttonstatus_e;

using namespace stm32plus;



class testclass
{
public:
    GpioC<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,14> > pc;

    void test(void)
    {
        pc.getSelectedPin();
        AutoRepeatPushButton button2(pc[14],true,999999,150);
    }
    AutoRepeatPushButton *button2();

private:
};

template<class T, class B>
class buttonpoti{

public:
    //buttonpoti(GeneralPurposeTimer &Timer);
    buttonpoti(bool isrepeat = false);
    void setpwm(uint8_t Duty);
    void init(void);
    bool getpushbutton(void);
    void setPotiVal(uint16_t PotiVal);
    void Handler(void);

    //GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,7> > pb;
    B pb;

private:

    int8_t getActivePinHandler(void);

    T timer;
    AutoRepeatPushButton *pushbutton;
    //buttonstatus_e buttonstatus;
    uint16_t PotiVal;
    uint16_t PwmVal;
    bool wasButtonPressed = false;
    bool isrepeatButton;
    int8_t ActivePinHandler;
};

template <class T, class B>
buttonpoti<T, B>::buttonpoti(bool isrepeat)
{
    ActivePinHandler = getActivePinHandler();

    timer.setTimeBaseByFrequency(100000, 255);
    timer.initCompareForPwmOutput(0, TIM_OCMode_PWM1, TIM_OCPolarity_Low);
    timer.enablePeripheral();

    isrepeatButton = isrepeat;
    if (isrepeatButton == true)
        {
            if(ActivePinHandler !=-1)
                {
                    pushbutton = new AutoRepeatPushButton(pb[ActivePinHandler],true,5000,150);
                    //pushbutton = new AutoRepeatPushButton(pb[Pin],true,999999,150);
                }
        }


    PotiVal = 0;
    PwmVal  = 0;
    if (pb.read() == Bit_SET)
        {
            wasButtonPressed = false;
        }
}

template <class T, class B>
void buttonpoti<T, B>::setpwm(uint8_t Duty)
{
    timer.setDutyCycle(Duty);
}

template <class T, class B>
bool buttonpoti<T, B>::getpushbutton(void)
{
if (isrepeatButton)
    {
        if (pushbutton->getState() == PushButton::Pressed)
            return true;
    }
    return false;
}

template <class T, class B>
void buttonpoti<T, B>::setPotiVal(uint16_t PotiVal)
{
    buttonpoti<T,B>::PotiVal = PotiVal;
}

template <class T, class B>
void buttonpoti<T,B>::Handler(void)
{
    PwmVal = (PotiVal * 100) / 255;
    if (isrepeatButton)
        {
        if(pushbutton->getState() == PushButton::Pressed)
            {
                wasButtonPressed = !wasButtonPressed;
            }

        if (wasButtonPressed)
            {
                PwmVal = 0;
            }
        }
    else
        {
            if (ActivePinHandler != -1)
                {
                    if (pb[ActivePinHandler].read()==Bit_SET)
                        {
                            PwmVal = 0;
                        }
                }
        }
    setpwm(PwmVal);
}

template <class T, class B>
int8_t buttonpoti<T,B>::getActivePinHandler(void)
{
    for (int i = 0; i < 16; i++)
        {
            if (pb._pinHandlers[i] != 0)
                {
                    return i;
                }
        }
    return -1;
}
