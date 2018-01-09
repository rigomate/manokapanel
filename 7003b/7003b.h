/*
 * 7003b.h
 *
 *  Created on: 2017. okt. 10.
 *      Author: mester
 */




#include "config/stm32plus.h"
#include "config/usart.h"

extern "C" {
#include "mano_font.h"
}

using namespace stm32plus;

/* This USART setup can be found in the follwing file:
 * */
struct UsartSetup {
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

#define busyPin busypin[12]
#define resetPin resetpin[10]



class display_7003b {

public:
    void run(void);
    void init(void);
    void sendhello(void);
    void sendstring(char *text);

    void screensaver(void);
    void clear(void);
    void character(char *character);
    void bitmap(void);
    void horizontal_scroll(void);
    void define_user_window(uint8_t window_num, uint16_t left, uint16_t top, uint16_t x_size, uint16_t y_size);
    void select_user_window(uint8_t window_num);
    void cursor_set(uint16_t x_pos, uint16_t y_pos);
    void draw_own_char(char character, const tFont *Font);
    void draw_own_string(char *string, const tFont *Font);
    //Constructor
    display_7003b(uint8_t id = 0) : window_id(id), cursor_pos_x(0){}

    //Destructor
    ~display_7003b();
protected:
    uint8_t window_id = 0;
    uint16_t cursor_pos_x;

    uint8_t getwindow_id(void);
    uint16_t get_cursor_pos_x(void);
    void set_cursor_pos_x(uint16_t cursor_pos);

private:
    void initDisplay(void);
    void sendcommand(uint8_t byte);

    /*These will be created by the constructor, but access is required elsewhere also
     * So we create define a pointer in the class body
     */

    Usart1_Custom<UsartSetup> *usart;
    static UsartPollingOutputStream *outputStream;


    GpioA<DefaultDigitalOutputFeature<10> > resetpin;
    GpioA<DefaultDigitalInputFeature<12> > busypin;

};

class display_7003b_user_window: public display_7003b{
public:
    display_7003b_user_window(uint8_t id = 1) : display_7003b(id){define_user_window(window_id, 35, 0, 56, 4);}

    void activate(void);

protected:

private:
};

