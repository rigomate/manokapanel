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

extern "C" {
#include "fwlib/f1/stdperiph/inc/stm32f10x_rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
}

uint16_t color1;
uint16_t c2;
uint16_t c3;

/*
 * Main entry point
 */
volatile int debugmaki;
static void prvStepperTask( void *pvParameters )
{

    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,4> > pb;
    stepper_drv8806 stepper;
    for( ;; )
    {
        debugmaki++;

        if (!pb[4].read())
            {
                stepper.stepPositive();
            }

        vTaskDelay(5);
    }
}
QueueHandle_t xQueuePoti[7];
volatile int debugqueue;
uint16_t potidebug[7];
uint16_t pwmval;
static void prvLedTask( void *pvParameters )
{


    pwm pwmc;
    ws2812 colorled;

    GpioA<DefaultDigitalOutputFeature<15> > pa;
    GpioB<DefaultDigitalOutputFeature<3> > pb_out;
    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,5,6,7,8,9> > pb;
    GpioC<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,14> > pc;
    for( ;; )
    {
        //poti.startConversion();

        uint16_t tempval;

        for (int i = 0; i< 7;i++)
            {
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
        vTaskDelay(50 / portTICK_PERIOD_MS);
        colorled.setcolor(0, color1, c2, c3);
        colorled.setcolor(1, c2, c3, color1);
        colorled.setcolor(2, c3, color1, c2);


        pwmval = (potidebug[5] * 100)/ 255;
        pwmc.setpwm(32, pwmval);

        pwmval = (potidebug[6] * 100)/ 255;
        if (pb[7].read()) pwmval = 0;
        pwmc.setpwm(33, pwmval);

        pwmval = (potidebug[2] * 100)/ 255;
        if (pb[9].read()) pwmval = 0;
        pwmc.setpwm(14, pwmval);

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

volatile int direction = 0;
volatile RCC_ClocksTypeDef RCC_Clocks;
uint32_t debugval;

int main() {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_Clocks);

  display_7003b test;

  //stepper_drv8806 stepper;
  //MillisecondTimer::initialise();
  //std::string maki("maki");
  //test.sendstring("maki");
  //ws2812 colorled;
  //potmeter poti;



  test.bitmap();
  test.horizontal_scroll();

  xTaskCreate( prvStepperTask, "Stepper", 512, NULL, 10, NULL );
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
