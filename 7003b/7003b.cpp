/*
 * 7003b.cpp
 *
 *  Created on: 2017. okt. 10.
 *      Author: mester
 */
#include "7003b.h"

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

//Constructor
display_7003b::display_7003b()
{
    UsartPeripheral<UsartSetup,PERIPHERAL_USART1>::Parameters usartparams;
    usartparams.usart_baudRate = 115200;
    usartparams.usart_synchronous = true;
    usartparams.usart_mode = USART_Mode_Tx;

    usart = new Usart1_Custom<UsartSetup>(usartparams);
    outputStream = new UsartPollingOutputStream (*usart);

    initDisplay();

    /* Send some example stuff */
    sendstring("Rigo Lilien");
}

//Destructor
display_7003b::~display_7003b()
{
    //Todo deinit uart
}

void display_7003b::sendstring(char *text)
{
    outputStream->write(text,strlen(text));
}
