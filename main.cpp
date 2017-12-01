/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "7003b/7003b.h"
#include "stepper/stepper.h"
#include "ws2812/ws2812_class.h"
#include "potmeter/potmeter.h"

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

    stepper_drv8806 stepper;
    for( ;; )
    {
        debugmaki++;


        stepper.stepPositive();
        vTaskDelay(5);
    }
}
//QueueHandle_t xQueuePoti[7];
//uint16_t potidebug[7];
static void prvLedTask( void *pvParameters )
{

    ws2812 colorled;
    //potmeter poti;
    for( ;; )
    {
            //poti.startConversion();

/*
            for (int i = 0; i< 7;i++)
                {
                    potidebug[i] = poti.getVal(i);
                }
                */
            /*
            uint16_t tempval;
            xQueueReceive(xQueuePoti[0], &tempval, (TickType_t) 0);
            color1 = tempval >>4;
            xQueueReceive(xQueuePoti[4], &tempval, (TickType_t) 0);
            c2 = tempval >>4;
            xQueueReceive(xQueuePoti[3], &tempval, (TickType_t) 0);
            c3 = tempval >>4;
            */
            //color1 = poti.getVal(0) >> 4;
            //c2 = (poti.getVal(4) >> 4);
            //c3 = (poti.getVal(3) >> 4);
            if (color1 >10) color1 = color1 /10 + 10;
            if (c2 >10) c2 = c2 /10 + 10;
            if (c3 >10) c3 = c3 /10 + 10;
            colorled.setcolor(0, color1, c2, c3);
            colorled.setcolor(1, c2, c3, color1);
            colorled.setcolor(2, c3, color1, c2);
        vTaskDelay(5);
    }
}


static void prvPotiTask( void *pvParameters )
{
    /*
    for (int i = 0; i < 7; i++)
        {
            xQueuePoti[i] = xQueueCreate( 0, sizeof( uint16_t ) );
        }
*/
    potmeter poti;
    for( ;; )
    {
            /*
        poti.startConversion();
        for (int i = 0; i < 7; i++)
            {
                uint16_t tempval = poti.getVal(i);
                xQueueSend(xQueuePoti[i], &tempval, ( TickType_t ) 0);
            }
*/
        vTaskDelay(5);
    }
}

volatile int direction = 0;
volatile RCC_ClocksTypeDef RCC_Clocks;
uint32_t debugval;

int main() {
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

  xTaskCreate( prvStepperTask, "Stepper", 128, NULL, 10, NULL );
  xTaskCreate( prvLedTask, "Led", 256, NULL, 1, NULL );
  xTaskCreate( prvPotiTask, "Poti", 256, NULL, 1, NULL );

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
