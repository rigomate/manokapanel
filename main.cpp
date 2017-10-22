/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "7003b/7003b.h"
#include "stepper/stepper.h"


/*
 * Main entry point
 */

volatile int direction = 0;
int main() {

  display_7003b test;
  stepper_drv8806 stepper;
  MillisecondTimer::initialise();
  //std::string maki("maki");
  //test.sendstring("maki");

  while(1)
      {
          if (direction == 0)
              {
                  stepper.stepPositive();
              }
          else
          {
              stepper.stepNegative();
          }
          MillisecondTimer::delay(5);
      }
  // not reached
  return 0;
}
