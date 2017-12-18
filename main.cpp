/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "7003b/7003b.h"
#include "stepper/stepper.h"
#include "ws2812/ws2812_class.h"
#include "potmeter/potmeter.h"
#include "pwm/pwm.h"
#include "config/button.h"
#include "buttonpoti/buttonpoti.h"
#include "config/timing.h"


extern "C" {
#include "fwlib/f1/stdperiph/inc/stm32f10x_rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
}

uint16_t color1;
uint16_t c2;
uint16_t c3;


typedef Timer3<TimerChannel3Feature<>,
        Timer3InternalClockFeature,
        Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH3_OUT> > timer3_3;
typedef GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,7> > Button_PB7;

typedef Timer3<TimerChannel2Feature<>,
        Timer3InternalClockFeature,
        Timer3GpioFeature<TIMER_REMAP_NONE, TIM3_CH2_OUT> > timer3_2;
typedef GpioC<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,14> > Button_PC14;

typedef Timer1<TimerChannel4Feature<>,
        Timer1InternalClockFeature,
        Timer1GpioFeature<TIMER_REMAP_NONE, TIM1_CH4_OUT> > timer1_4;
typedef GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,9> > Button_PB9;

QueueHandle_t xQueuePoti[7];
QueueHandle_t xQueueButton[7];
volatile int debugqueue;
uint16_t potidebug[7];
uint16_t pwmval;

static int16_t delay;
static int16_t delayabs;
static uint16_t delaycounter = 0;
uint16_t delaydebug;
static void prvStepperTask( void *pvParameters )
{

    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,4> > pb;
    stepper_drv8806 stepper;

    uint16_t poti;
    //static int16_t delay;
    //static uint16_t delaycounter = 0;
    BaseType_t queuerecieve;
    bool shouldstop = false;

    for( ;; )
    {

    if (xQueuePoti[1] != NULL)
        {
            queuerecieve = xQueueReceive(xQueuePoti[1], &poti, (TickType_t) 0);
        }
    if(queuerecieve == pdTRUE)
        {
            delay = poti - (4096 / 2);
            delay = delay / 5;
            if (delay > 0)
                {
                    delay = 410 - delay;
                }
            else
                {
                    delay = -410 - delay;
                }
        }


    delayabs = abs(delay);
    if (delayabs < 100)
        {
            delayabs = delayabs / 10 + 5;
        }
    else
        {
            delayabs =  delayabs - 100 +10 + 5;
        }
    if (delayabs < 5)
        {
            delayabs = 5;
        }
    if (delayabs > 320)
        {
            delaycounter = 0;
            shouldstop = true;
        }
    else
        {
            shouldstop = false;
        }

    delaydebug = delayabs;

    if (delaycounter > delayabs)
        {
            delaycounter = 0;
            if (delay > 0)
                {
                    stepper.stepNegative();
                }
            else
                {
                    stepper.stepPositive();
                }
        }

    if (!pb[4].read() && shouldstop == false)
    {
        stepper.Enable();
    }
    else
    {
        stepper.Disable();
    }

    delaycounter++;
        vTaskDelay(1);
    }
}

static void prvLedTask( void *pvParameters )
{

    uint16_t display_counter = 0;
    char asciistring[10];

    pwm pwmc;
    vTaskSuspendAll();
    ws2812 colorled;
    display_7003b display;


    GpioA<DefaultDigitalOutputFeature<15> > pa;
    pa.setAll();
    GpioB<DefaultDigitalOutputFeature<3> > pb_out;
    pb_out.setAll();
    xTaskResumeAll();
    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,5,6,8,9> > pb;
    GpioC<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,14> > pc;

    AutoRepeatPushButton button(pb[8],true,999999,150);

    AutoRepeatPushButton button_counter(pb[5],true,5000,150);

    buttonpoti<timer3_3, Button_PB7> buttonpoti1(false);
    buttonpoti<timer3_2, Button_PC14> buttonpoti2(true);
    buttonpoti<timer1_4, Button_PB9> buttonpoti3(false);

    display.bitmap();
    display.horizontal_scroll();

    for( ;; )
    {
        //poti.startConversion();

        uint16_t tempval;

        for (int i = 0; i< 7;i++)
            {
                if (i == 1) continue;
                xQueueReceive(xQueuePoti[i], &tempval, (TickType_t) 0);
                potidebug[i] = tempval >> 4;
            }


        //debugqueue = xQueueReceive(xQueuePoti[0], &tempval, (TickType_t) 0);
        color1 = potidebug[0];
        //debugqueue = xQueueReceive(xQueuePoti[4], &tempval, (TickType_t) 0);
        c2 = potidebug[4];
        //debugqueue = xQueueReceive(xQueuePoti[3], &tempval, (TickType_t) 0);
        c3 = potidebug[3];


        if (color1 >10) color1 = color1 /10 + 10;
        if (c2 >10) c2 = c2 /10 + 10;
        if (c3 >10) c3 = c3 /10 + 10;


        vTaskSuspendAll();
        colorled.setcolor(0, color1, c2, c3);
        xTaskResumeAll();

        vTaskSuspendAll();
        colorled.setcolor(1, c2, c3, color1);
        xTaskResumeAll();

        vTaskSuspendAll();
        colorled.setcolor(2, c3, color1, c2);
        xTaskResumeAll();

        buttonpoti2.setPotiVal(potidebug[5]);
        buttonpoti2.Handler();
        //pwmval = (potidebug[5] * 100)/ 255;
        //pwmc.setpwm(32, pwmval);


        buttonpoti1.setPotiVal(potidebug[6]);
        buttonpoti1.Handler();
        //pwmval = (potidebug[6] * 100)/ 255;
        //if (pb[7].read()) pwmval = 0;
        //pwmc.setpwm(33, pwmval);
        /*
        if (buttonpoti1.getpushbutton())
            {
                pwmval = 0;
            }
        buttonpoti1.setpwm(pwmval);
*/

        pwmval = (potidebug[2] * 100)/ 255;

        buttonpoti3.setPotiVal(potidebug[2]);
        buttonpoti3.Handler();
        //if (pb[9].read()) pwmval = 0;
        //pwmc.setpwm(14, pwmval);

        if (pb[6].read())
            {
                pa[15].reset();
            }
        else
            {
                pa[15].set();
            }

        if(pb[8].read())
            {
                pb_out[3].set();
            }
        else
            {
                pb_out[3].reset();
            }

        if(button.getState()==PushButton::Pressed)
            {
                display.bitmap();
                display.horizontal_scroll();
            }

        if (button_counter.getState() == PushButton::Pressed)
            {
                display.clear();
                snprintf(asciistring,9, "%d", display_counter++);
                display.character(asciistring);
            }
        if(pb[5].read())
            {
                pwmc.setpwm(34, 0);
            }
        else
            {
                pwmc.setpwm(34, 100);
            }

        vTaskDelay(5);
    }
}


static void prvPotiTask( void *pvParameters )
{

    for (int i = 0; i < 7; i++)
        {
            xQueuePoti[i] = xQueueCreate( 1, sizeof( uint16_t ) );
        }

    potmeter poti;
    for( ;; )
    {
        poti.startConversion();
        for (int i = 0; i < 7; i++)
            {
                uint16_t tempval = poti.getVal(i);
                debugqueue = xQueueSend(xQueuePoti[i], &tempval, ( TickType_t ) 0);
            }

        vTaskDelay(5);
    }
}

static void prvButtonTask( void *pvParameters )
{

    for (int i = 0; i < 7; i++)
        {
            xQueueButton[i] = xQueueCreate( 1, sizeof( uint16_t ) );
        }

    potmeter poti;
    for( ;; )
    {
        poti.startConversion();
        for (int i = 0; i < 7; i++)
            {
                uint16_t tempval = poti.getVal(i);
                debugqueue = xQueueSend(xQueuePoti[i], &tempval, ( TickType_t ) 0);
            }

        vTaskDelay(5);
    }
}

volatile int direction = 0;
volatile RCC_ClocksTypeDef RCC_Clocks;
uint32_t debugval;


int main() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_Clocks);

  //display_7003b test;

  //stepper_drv8806 stepper;
  //MillisecondTimer::initialise();
  //std::string maki("maki");
  //test.sendstring("maki");
  //ws2812 colorled;
  //potmeter poti;



  //test.bitmap();
  //test.horizontal_scroll();

  xTaskCreate( prvStepperTask, "Stepper", 1024, NULL, 9, NULL );
  xTaskCreate( prvLedTask, "Led", 512, NULL, 1, NULL );
  xTaskCreate( prvPotiTask, "Poti", 256, NULL, 5, NULL );

  /* Start the scheduler. */
    vTaskStartScheduler();
  while(1)
      {
/*
          if (direction == 0)
              {
                  stepper.stepPositive();
              }
          else
          {
              stepper.stepNegative();
          }

          MillisecondTimer::delay(5);
          poti.startConversion();


          color1 = poti.getVal(0) >> 4;
          c2 = (poti.getVal(1) >> 4);
          c3 = (poti.getVal(2) >> 4);

          if (color1 >100) direction =1;
          else direction =0;

          if (color1 >10) color1 = color1 /10 + 10;
          if (c2 >10) c2 = c2 /10 + 10;
          if (c3 >10) c3 = c3 /10 + 10;
          colorled.setcolor(0, color1, c2, c3);
          colorled.setcolor(1, c2, c3, color1);
          colorled.setcolor(2, c3, color1, c2);
          */
      }
  // not reached
  return 0;
}

extern "C" {
    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        signed char *pcTaskName )
    {
        for(;;)
            {
                __NOP();
            }
    }
}
