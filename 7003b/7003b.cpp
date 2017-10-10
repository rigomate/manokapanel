/*
 * 7003b.cpp
 *
 *  Created on: 2017. okt. 10.
 *      Author: mester
 */
#include "7003b.h"

void display_7003b::sendhello(void) {

  const char *dataToSend="Hello World";

  /*
   * Declare a USART1 object. Note that an alternative Usart1_Remap object is available
   * if your application demands that you use the alternate pins for USART1. All the other
   * USART ports are available and types such as Usart1_Custom allow you to completely
   * customise your peripheral pinout if your MCU supports it.
   */
  UsartPeripheral<UsartOwnPackage,PERIPHERAL_USART1>::Parameters usartparams;
  usartparams.usart_baudRate = 57600;
  usartparams.usart_synchronous = true;

  //Usart1_Custom<UsartOwnPackage> usart(UsartPeripheral<UsartOwnPackage,PERIPHERAL_USART1>::Parameters(57600, true));
  Usart1_Custom<UsartOwnPackage> usart(usartparams);

  /*
   * For kicks we'll use an output stream for sending to the port instead of using the
   * send(uint8_t) method on the usart object
   */

  UsartPollingOutputStream outputStream(usart);

  /*
   * Send the data
   */

  if(!outputStream.write(dataToSend,strlen(dataToSend))) {
    // error handling would go here
  }

  // finished

  for(;;);
}

/* sends a command to the display */
void display_7003b::sendcommand(uint8_t byte)
{
    while(!busypin.read());
    outputStream->write(byte);
}

//Actual display initializer stuff
void display_7003b::initDisplay(void)
{
    MillisecondTimer::initialise();

    resetpin[10].reset();
    MillisecondTimer::delay(10);
    resetpin[10].set();
    MillisecondTimer::delay(10);

    sendcommand(0x1b);
    sendcommand(0x40);
    sendcommand(0x1f);
    sendcommand(0x28);
    sendcommand(0x61);
    sendcommand(0x40);
    sendcommand(0x04);
}

display_7003b::display_7003b()
{
    /*
     * Declare a USART1 object. Note that an alternative Usart1_Remap object is available
     * if your application demands that you use the alternate pins for USART1. All the other
     * USART ports are available and types such as Usart1_Custom allow you to completely
     * customise your peripheral pinout if your MCU supports it.
     */

    UsartPeripheral<UsartOwnPackage,PERIPHERAL_USART1>::Parameters usartparams;
    usartparams.usart_baudRate = 115200;
    usartparams.usart_synchronous = true;
    usartparams.usart_mode = USART_Mode_Tx;

    usart = new Usart1_Custom<UsartOwnPackage>(usartparams);

    outputStream = new UsartPollingOutputStream (*usart);

    //init the reset pin
    //GpioPinInitialiser::initialise(GPIOA, 10, Gpio::OUTPUT, GPIO_Speed_10MHz, Gpio::PUPD_NONE, Gpio::PUSH_PULL);

    initDisplay();

    /* Send some example stuff */
    sendstring("c++ class driver");
}

display_7003b::~display_7003b()
{
    //Todo deinit uart
}

void display_7003b::sendstring(char *text)
{
    outputStream->write(text,strlen(text));
}
