/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "7003b/7003b.h"
#include "stepper/stepper.h"
#include "ws2812/ws2812_class.h"

extern "C" {
#include "fwlib/f1/stdperiph/inc/stm32f10x_rcc.h"
}



/*
 * Main entry point
 */

volatile int direction = 0;
volatile RCC_ClocksTypeDef RCC_Clocks;
uint32_t debugval;
int main() {
    RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_Clocks);

  display_7003b test;
  //stepper_drv8806 stepper;
  MillisecondTimer::initialise();
  //std::string maki("maki");
  //test.sendstring("maki");
  ws2812 colorled;

  colorled.setcolor(0, 255, 0, 0);

  test.bitmap();
  test.horizontal_scroll();
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
*/
          //MillisecondTimer::delay(5);
          debugval = *((uint32_t*)0x40000040);
          if(debugval != 0x1E && debugval != 0x3c && debugval != 0x00)
              {
                  direction++;
              }
      }
  // not reached
  return 0;
}
