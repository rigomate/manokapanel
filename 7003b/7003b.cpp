/*
 * 7003b.cpp
 *
 *  Created on: 2017. okt. 10.
 *      Author: mester
 */
#include "7003b.h"
#include <vector>

static const uint8_t image_data_viragok_kis[1024] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x20, 0x00, 0x00,
    0x00, 0x60, 0x00, 0x00,
    0x00, 0x70, 0x00, 0x00,
    0x00, 0xd0, 0x00, 0x00,
    0x00, 0x98, 0x00, 0x00,
    0x00, 0x8c, 0x00, 0x00,
    0x00, 0x86, 0x00, 0x00,
    0x00, 0x83, 0x00, 0x00,
    0x00, 0x81, 0x80, 0x00,
    0x00, 0x88, 0x80, 0x00,
    0x00, 0xcc, 0x80, 0x00,
    0x00, 0x66, 0x80, 0x00,
    0x00, 0x33, 0x80, 0x00,
    0x00, 0x1f, 0x80, 0x00,
    0x00, 0x00, 0xff, 0xf0,
    0x00, 0x7c, 0xff, 0xf0,
    0x01, 0xc3, 0x80, 0x00,
    0x03, 0x07, 0x00, 0x00,
    0x02, 0x1d, 0x00, 0x00,
    0x04, 0x31, 0x00, 0x00,
    0x0c, 0x01, 0x00, 0x00,
    0x08, 0x03, 0x00, 0x00,
    0x08, 0x06, 0x00, 0x00,
    0x08, 0x0c, 0x00, 0x00,
    0x08, 0x30, 0x00, 0x00,
    0x08, 0x60, 0x00, 0x00,
    0x08, 0xc0, 0x00, 0x00,
    0x09, 0x00, 0x00, 0x00,
    0x0e, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0xf0, 0x00,
    0x00, 0x0e, 0x38, 0x00,
    0x00, 0x58, 0x0d, 0x00,
    0x03, 0xf0, 0x07, 0xe0,
    0x06, 0x18, 0x0c, 0x30,
    0x0c, 0x08, 0x08, 0x18,
    0x08, 0x08, 0x88, 0x08,
    0x18, 0x0c, 0x98, 0x0c,
    0x18, 0x04, 0x90, 0x0c,
    0x18, 0x66, 0xb3, 0x0c,
    0x1c, 0x32, 0xa6, 0x1c,
    0x37, 0x1b, 0xec, 0x76,
    0x61, 0xce, 0x39, 0xc3,
    0x40, 0x7c, 0x1f, 0x01,
    0x40, 0x08, 0x08, 0x01,
    0x41, 0xf8, 0x0f, 0xc1,
    0x40, 0x08, 0x08, 0x01,
    0x40, 0x18, 0x0c, 0x01,
    0x60, 0xec, 0x1b, 0x83,
    0x23, 0x8f, 0xf8, 0xe2,
    0x1e, 0x1b, 0xec, 0x3c,
    0x18, 0x36, 0xb6, 0x0c,
    0x18, 0x64, 0x93, 0x0c,
    0x18, 0x04, 0x90, 0x0c,
    0x08, 0x0c, 0x98, 0x08,
    0x0c, 0x08, 0x88, 0x18,
    0x06, 0x18, 0x0c, 0x30,
    0x03, 0x90, 0x04, 0xe0,
    0x01, 0xf0, 0x07, 0xc0,
    0x00, 0x0c, 0x18, 0x00,
    0x00, 0x07, 0xf0, 0x00,
    0x00, 0x01, 0xc0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x78, 0x00,
    0x18, 0x03, 0xfe, 0x00,
    0x0f, 0xfe, 0x03, 0x10,
    0x0c, 0xdc, 0x01, 0x30,
    0x0c, 0x10, 0x01, 0xf0,
    0x76, 0x30, 0x00, 0x90,
    0x42, 0x60, 0x00, 0xb0,
    0x47, 0xc0, 0x00, 0xe0,
    0x6d, 0x80, 0x00, 0xc0,
    0x39, 0x80, 0x00, 0xff,
    0x19, 0x00, 0x00, 0xff,
    0x13, 0x00, 0x00, 0xc0,
    0x32, 0x00, 0x00, 0xe0,
    0x36, 0x00, 0x00, 0xb0,
    0x34, 0x00, 0x00, 0x90,
    0x3c, 0x00, 0x01, 0xf0,
    0x08, 0xe0, 0x01, 0x30,
    0x0f, 0xfe, 0x03, 0x10,
    0x18, 0x03, 0xfe, 0x00,
    0x00, 0x00, 0x78, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x00,
    0x00, 0xe0, 0x00, 0x00,
    0x00, 0xbc, 0x00, 0x00,
    0x00, 0x87, 0x80, 0x00,
    0x00, 0x80, 0xe0, 0x00,
    0x0c, 0x80, 0x30, 0x00,
    0x0f, 0xff, 0x18, 0x00,
    0x04, 0x03, 0xec, 0x00,
    0x04, 0x00, 0x3e, 0x00,
    0x06, 0x00, 0x0e, 0x00,
    0x02, 0x00, 0x03, 0x00,
    0x07, 0x00, 0x03, 0x00,
    0x05, 0x80, 0x01, 0x00,
    0x08, 0xc0, 0x01, 0x80,
    0x18, 0x78, 0x01, 0x80,
    0x60, 0x0f, 0xff, 0xff,
    0x30, 0x3f, 0x01, 0x80,
    0x18, 0x60, 0x01, 0x80,
    0x0d, 0xc0, 0x01, 0x80,
    0x07, 0x00, 0x01, 0x00,
    0x06, 0x00, 0x03, 0x00,
    0x06, 0x00, 0x07, 0x00,
    0x04, 0x00, 0x0e, 0x00,
    0x04, 0x00, 0x7c, 0x00,
    0x05, 0xff, 0xcc, 0x00,
    0x0f, 0xf0, 0x18, 0x00,
    0x00, 0x80, 0x60, 0x00,
    0x00, 0x83, 0xc0, 0x00,
    0x00, 0x9e, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00,
    0x00, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x80, 0x00,
    0x00, 0x3f, 0xf0, 0x00,
    0x00, 0xe0, 0x1c, 0x00,
    0x03, 0x80, 0x06, 0x00,
    0x06, 0x00, 0x03, 0x00,
    0x0c, 0x00, 0x01, 0x80,
    0x18, 0x00, 0x00, 0x80,
    0x30, 0x18, 0x60, 0xc0,
    0x60, 0x0c, 0x30, 0x40,
    0x40, 0x06, 0x18, 0x60,
    0x40, 0x7f, 0xff, 0xff,
    0x40, 0x06, 0x18, 0x60,
    0x60, 0x0c, 0x30, 0x40,
    0x30, 0x18, 0x60, 0xc0,
    0x18, 0x00, 0x00, 0x80,
    0x0c, 0x00, 0x01, 0x80,
    0x06, 0x00, 0x03, 0x00,
    0x03, 0x80, 0x06, 0x00,
    0x00, 0xe0, 0x1c, 0x00,
    0x00, 0x3f, 0xf0, 0x00,
    0x00, 0x07, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x01, 0xf3, 0xe0, 0x00,
    0x03, 0x1e, 0x30, 0x00,
    0x02, 0x04, 0x10, 0x00,
    0x02, 0x00, 0x10, 0x00,
    0x03, 0x00, 0x10, 0x00,
    0x3f, 0x00, 0x3f, 0x00,
    0x67, 0x80, 0x39, 0x80,
    0x41, 0xc0, 0x60, 0xc0,
    0x40, 0xc0, 0xc0, 0xc0,
    0x40, 0x61, 0x80, 0xc0,
    0x60, 0x33, 0x00, 0x80,
    0x20, 0x00, 0x01, 0x80,
    0x30, 0x00, 0x01, 0xff,
    0x60, 0x00, 0x01, 0x80,
    0x40, 0x33, 0x00, 0x80,
    0x40, 0x61, 0x80, 0xc0,
    0x40, 0xc0, 0xe0, 0xc0,
    0x63, 0x80, 0x70, 0xc0,
    0x3f, 0x00, 0x3f, 0x80,
    0x1b, 0x00, 0x12, 0x00,
    0x02, 0x00, 0x10, 0x00,
    0x02, 0x00, 0x18, 0x00,
    0x03, 0x0e, 0x10, 0x00,
    0x01, 0xbb, 0xb0, 0x00,
    0x00, 0xe1, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0xe0, 0x00,
    0x00, 0x7c, 0x78, 0x00,
    0x01, 0xc0, 0x0e, 0x00,
    0x03, 0x00, 0x03, 0x00,
    0x0e, 0x18, 0x61, 0x80,
    0x18, 0x0c, 0x30, 0xc0,
    0x70, 0x06, 0x18, 0x60,
    0x60, 0x3f, 0xff, 0xff,
    0x30, 0x06, 0x18, 0x60,
    0x1c, 0x0c, 0x30, 0xc0,
    0x06, 0x08, 0x21, 0x80,
    0x03, 0x80, 0x07, 0x00,
    0x00, 0xf0, 0x1c, 0x00,
    0x00, 0x3f, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x38, 0x00,
    0x00, 0x40, 0x2c, 0x00,
    0x00, 0x70, 0x26, 0x00,
    0x00, 0x58, 0x23, 0x00,
    0x60, 0xcc, 0x21, 0x00,
    0x70, 0xc4, 0x21, 0x80,
    0x58, 0x46, 0x20, 0x80,
    0x48, 0x42, 0x30, 0x80,
    0x4c, 0x62, 0x10, 0x80,
    0x64, 0x32, 0x08, 0x80,
    0x3c, 0x1e, 0x0f, 0x80,
    0x1f, 0xff, 0xff, 0xff,
    0x3c, 0x1e, 0x0f, 0x80,
    0x64, 0x32, 0x08, 0x80,
    0x4c, 0x62, 0x10, 0x80,
    0x48, 0x42, 0x30, 0x80,
    0x58, 0x46, 0x20, 0x80,
    0x70, 0xc4, 0x21, 0x80,
    0x60, 0xcc, 0x21, 0x00,
    0x00, 0x58, 0x23, 0x00,
    0x00, 0x70, 0x26, 0x00,
    0x00, 0x40, 0x2c, 0x00,
    0x00, 0x00, 0x38, 0x00,
    0x00, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xe0, 0x00,
    0x00, 0x23, 0x21, 0x00,
    0x00, 0xf2, 0x37, 0x80,
    0x01, 0x8e, 0x1c, 0x40,
    0x01, 0x8e, 0x18, 0x40,
    0x01, 0x86, 0x10, 0x40,
    0x0e, 0x82, 0x30, 0xd8,
    0x1f, 0xc3, 0x20, 0xfc,
    0x10, 0xc3, 0x21, 0x86,
    0x10, 0x61, 0x63, 0x02,
    0x10, 0x31, 0xc2, 0x06,
    0x0c, 0x18, 0xc6, 0x0c,
    0x0e, 0x0d, 0xcc, 0x38,
    0x3f, 0xff, 0x7f, 0xff,
    0x60, 0x0e, 0x1c, 0x01,
    0x40, 0x06, 0x10, 0x01,
    0x40, 0x06, 0x10, 0x01,
    0x70, 0xfe, 0x3f, 0x83,
    0x1f, 0xef, 0xf9, 0xfe,
    0x0c, 0x18, 0xcc, 0x18,
    0x18, 0x11, 0xc6, 0x04,
    0x10, 0x21, 0x43, 0x06,
    0x30, 0x61, 0x61, 0x82,
    0x11, 0xc3, 0x21, 0xe6,
    0x0f, 0x82, 0x30, 0xfc,
    0x00, 0x86, 0x10, 0x40,
    0x01, 0x86, 0x18, 0x40,
    0x01, 0x8e, 0x18, 0x40,
    0x00, 0xda, 0x16, 0xc0,
    0x00, 0x72, 0x33, 0x80,
    0x00, 0x03, 0xe0, 0x00,
    0x00, 0x00, 0xc0, 0x00,
    0x00, 0x00, 0x00, 0x00
};


/* sends a command to the display */
void display_7003b::sendcommand(uint8_t byte)
{
    while(!busypin.read());
    outputStream->write(byte);
}

//Actual display initializer stuff
void display_7003b::initDisplay(void)
{


    resetpin[10].reset();
    for (int i = 0; i< 500000; i++)
        {
            __NOP();
        }
    resetpin[10].set();
    for (int i = 0; i< 500000; i++)
        {
            __NOP();
        }

    //ESC @ init Initialize Display
    sendcommand(0x1b);
    sendcommand(0x40);
}

void display_7003b::screensaver(void)
{
    //Function 40h Screen Saver
    sendcommand(0x1f);
    sendcommand(0x28);
    sendcommand(0x61);
    sendcommand(0x40);

    //Repeat blink display
    sendcommand(0x04);
}

void display_7003b::horizontal_scroll(void)
{
#if 0
    std::vector<int> v = {1,2,3,4};
    std::vector<uint8_t> command;/* = {
            0x1f,
            0x28,
            0x61,
            0x10,
            0x04,
            0x00
            0xff,
            0x00,
            0x01
    };
*/
#endif
    sendcommand(0x1f);
    sendcommand(0x28);
    sendcommand(0x61);
    sendcommand(0x10);

    sendcommand(4);
    sendcommand(0);

    //cycles
    sendcommand(255);
    sendcommand(0);

    sendcommand(1);
}

void display_7003b::bitmap(void)
{
    //set all screen mode
    sendcommand(0x1f);
    sendcommand(0x28);
    sendcommand(0x77);
    sendcommand(0x10);
    sendcommand(0x01);

    sendcommand(0x1f);
    sendcommand(0x28);
    sendcommand(0x66);
    sendcommand(0x11);

    sendcommand(0);
    sendcommand(1);

    sendcommand(32 / 8);
    sendcommand(0);

    sendcommand(1);

    //8 pixels are sent horizontically at once
    for(int i = 0; i < 1024; i++)
        {
            sendcommand(image_data_viragok_kis[i]);
        }
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
    //sendstring("Rigo Lilien");
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
