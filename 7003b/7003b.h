/*
 * 7003b.h
 *
 *  Created on: 2017. okt. 10.
 *      Author: mester
 */




#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


struct UsartOwnPackage {
  enum {
    Port_CK=GPIOA_BASE,
    Port_TX=GPIOA_BASE,
    Port_RX=GPIOA_BASE,
    Port_CTS=GPIOA_BASE,
    Port_RTS=GPIOA_BASE,

    Pin_CK=GPIO_Pin_8,
    Pin_TX=GPIO_Pin_9,
    Pin_RX=GPIO_Pin_10,
    Pin_CTS=GPIO_Pin_11,
    Pin_RTS=GPIO_Pin_12
  };
};


class display_7003b {

public:
    void run(void);
    void init(void);
    void sendhello(void);
    void sendstring(char *text);

    //Constructor
    display_7003b();

    //Destructor
    ~display_7003b();

private:
    void initDisplay(void);
    void sendcommand(uint8_t byte);

    Usart1_Custom<UsartOwnPackage> *usart;
    UsartPollingOutputStream *outputStream;
    GpioA<DefaultDigitalOutputFeature<10> > resetpin;
    GpioA<DefaultDigitalInputFeature<12> > busypin;
};
