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

#include "game-of-life.h"
#include "emwin-display.h"

extern "C" {
#include "fwlib/f1/stdperiph/inc/stm32f10x_rcc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "GUI.h"
#include "semphr.h"
#include "SEGGER_SYSVIEW.h"
}

#include <vector>

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
QueueHandle_t xQueuePoti2[7];
QueueHandle_t xQueueButton[7];
volatile int debugqueue;
uint16_t potidebug[7];
uint16_t pwmval;

static int16_t delay;
static int16_t delayabs;
static uint16_t delaycounter = 0;
uint16_t delaydebug;

int32_t normalizepoti(uint16_t poti)
{
	int32_t val = poti - (4096 / 2);
	if (val < 200 && val > -200)
	{
		val = 0;
	}
	val = (val * 100) / (4096 / 2);
	return val;
}
template <typename Usart>
void UartSend(Usart usart, const char* format, ...) {
    va_list args;

	char buffer[20]{0};
	char *bufferptr = buffer;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
	while(*bufferptr != 0)
	{
		usart.send(*bufferptr);
		bufferptr++;
	}
}

char* itoa(int value, char* result, int base) {
    // Check for invalid base
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    // Handle 0 explicitly, otherwise empty string is returned
    if (value == 0) {
        *result++ = '0';
        *result = '\0';
        return result - 1;
    }

    // Handle negative numbers for bases other than 10
    if (value < 0 && base != 10) {
        *result++ = '-';
        value = -value;
    }

    // Process individual digits
    while (value != 0) {
        int rem = value % base;
        *result++ = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value /= base;
    }

    *result = '\0';

    // Reverse the string
    char* start = result - 1;
    char* end = result - 1;

    while (start > result - 1 - (value < 0)) {
        char temp = *start;
        *start-- = *end;
        *end-- = temp;
    }

    return result - 1;
}

static void prvStepperTask( void *pvParameters )
{
	(void)pvParameters;
    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,4> > pb;
    stepper_drv8806 stepper(1000 * 60 * 25);

    uint16_t poti;
    //static int16_t delay;
    //static uint16_t delaycounter = 0;
    BaseType_t queuerecieve = pdFALSE;
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
                    --stepper;
                }
            else
                {
                    ++stepper;
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

extern GUI_CONST_STORAGE GUI_BITMAP bmvirag;
SemaphoreHandle_t xSemaphore = NULL;
static void prvDisplayTask( void *pvParameters )
{
	(void)pvParameters;
#if 0
    display_7003b display;
    display.init();

    display_7003b_user_window user_window;

    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,5,8> > pb;
    AutoRepeatPushButton button(pb[8],true,999999,150);
    AutoRepeatPushButton button_counter(pb[5],true,5000,150);

    pwm pwmc;
    GpioB<DefaultDigitalOutputFeature<3> > pb_out;

    char asciistring[10];
    uint32_t display_counter = 0;

    //pb_out.setAll();


    //display.define_user_window(1, 35, 0, 56, 4);
    display.select_user_window(0);
    display.bitmap();
    display.horizontal_scroll();
    for(;;)
    {

        if(button.getState()==PushButton::Pressed)
            {
                display.select_user_window(0);
                display.bitmap();
                display.horizontal_scroll();
            }

        if (button_counter.getState() == PushButton::Pressed)
            {
                //display.select_user_window(1);
                //display.clear();
                //display.cursor_set(0, 8);
                user_window.clear();
                user_window.cursor_set(0,8);
                snprintf(asciistring,9, "%d", display_counter++);
                if (display_counter > 99999)
                {
                    display_counter = 0;
                }
                //display.character(asciistring);
                //display.draw_own_string(asciistring, &ManoFont);
                user_window.draw_own_string(asciistring, &ManoFont);
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

        vTaskDelay(5 / portTICK_RATE_MS);
    }
#endif


    xSemaphore = xSemaphoreCreateBinary();
    configASSERT(xSemaphore != NULL);
    display_7003b display;
    display.init();

    display_7003b_user_window user_window;

    display.select_user_window(0);



    //GUI_DispString("Hello world!");
    //GUI_FillRect(0,0,127,31);
    //GUI_InvertRect(0,0,128,32);
    //GUI_DispString("Hello world!");
    //GUI_DrawLine(0,0,0,127);
    //GUI_DrawBitmap(&bmvirag, 0,0);


    for(;;)
    {
        if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE )
        {
            /* It is time to execute. */

            display.bitmap();

            /* We have finished our task.  Return to the top of the loop where
            we will block on the semaphore until it is time to execute
            again.  Note when using the semaphore for synchronisation with an
            ISR in this manner there is no need to 'give' the semaphore
            back. */
        }
    }
}

#define RECOMMENDED_MEMORY (102L * 5)

static void emwinTask( void *pvParameters ) {
  (void)pvParameters;
  int xPos = 0;
  uint32_t display_counter = 0;
  uint32_t gameoflife_counter = 0;
  bool doscroll = false;
  TickType_t xLastWakeTime;

  EmwinDisplay emwindisplay(128, 32);
  GameOfLife gameoflife(&emwindisplay, emwindisplay.getXwidth(),emwindisplay.getYwidth(), GameOfLife::RANDOM);
  bool gameoflifeinit = false;

  GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,5,8> > pb;
  AutoRepeatPushButton button(pb[8],true,999999,150);
  AutoRepeatPushButton button_counter(pb[5],true,5000,150);

  pwm pwmc;
  GpioB<DefaultDigitalOutputFeature<3> > pb_out;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
  GUI_Init();
  GUI_SetOrientation(GUI_SWAP_XY);


  GUI_SetTextMode(GUI_TM_REV);
  GUI_SetFont(GUI_FONT_20F_ASCII);
  //GUI_DispStringHCenterAt("Hello world!", xPos, yPos);
  //GUI_DispString("Hello");
  //GUI_SetFont(GUI_FONT_D24X32);



      // Initialise the xLastWakeTime variable with the current time.
  xLastWakeTime = xTaskGetTickCount ();


  xLastWakeTime = xTaskGetTickCount ();
  for (int i = 0; i < 128; i++)
  {
      GUI_MULTIBUF_Begin();
      GUI_Clear();
      GUI_DrawBitmap(&bmvirag, xPos--,0);
      GUI_MULTIBUF_End();
      vTaskDelayUntil( &xLastWakeTime, 35 / portTICK_RATE_MS );
  }

  GUI_MULTIBUF_Begin();
  GUI_Clear();
  GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
  GUI_DispStringAt("Lili Panel", 128/2, 16);
  GUI_MULTIBUF_End();

  while (1) {

      if(button.getState()==PushButton::Pressed)
          {
              xPos = 0;
              doscroll = true;
          }

      if (button_counter.getState() == PushButton::Pressed)
          {
              doscroll = false;
              GUI_MULTIBUF_Begin();
              GUI_Clear();
              GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
              GUI_DispDecAt(display_counter++, 128/2, 16, 8);
              GUI_MULTIBUF_End();
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

      xLastWakeTime = xTaskGetTickCount ();
      if (doscroll)
      {
          GUI_MULTIBUF_Begin();
          GUI_Clear();
          GUI_DrawBitmap(&bmvirag, xPos--,0);
          GUI_MULTIBUF_End();
          vTaskDelayUntil( &xLastWakeTime, 35 / portTICK_RATE_MS );
          if(xPos < -128)
          {
              doscroll = false;
          }
      }

      if (display_counter > 10)
      {
    	  if (gameoflifeinit == false)
    	  {
    		  gameoflife.SetRandom();
    		  gameoflifeinit = true;

    	  }

    	  if (gameoflife_counter > 100)
    	  {
    		  gameoflife_counter = 0;
        	  GUI_MULTIBUF_Begin();
        	  gameoflife.DrawTable();
        	  GUI_MULTIBUF_End();

        	  gameoflife.NextStep();
    	  }
    	  gameoflife_counter++;
      }
    vTaskDelay(5 / portTICK_RATE_MS);
  }
}

static void prvLedTask( void *pvParameters )
{
	(void)pvParameters;

    vTaskSuspendAll();
    ws2812 colorled;


    GpioA<DefaultDigitalOutputFeature<15> > pa;
    pa.setAll();

    xTaskResumeAll();
    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,6,9> > pb;
    GpioC<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,14> > pc;

    buttonpoti<timer3_3, Button_PB7> buttonpoti1(true);
    buttonpoti<timer3_2, Button_PC14> buttonpoti2(true);
    buttonpoti<timer1_4, Button_PB9> buttonpoti3(false);



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



        vTaskDelay(5);
    }
}


static void prvPotiTask( void *pvParameters )
{
	(void)pvParameters;
    for (int i = 0; i < 7; i++)
        {
            xQueuePoti[i] = xQueueCreate( 1, sizeof( uint16_t ) );
            configASSERT(xQueuePoti[i] != NULL);
        }
    for (int i = 0; i < 7; i++)
        {
            xQueuePoti2[i] = xQueueCreate( 1, sizeof( uint16_t ) );
            configASSERT(xQueuePoti2[i] != NULL);
        }

    potmeter poti;
    for( ;; )
    {
        poti.startConversion();
        for (int i = 0; i < 7; i++)
            {
                uint16_t tempval = poti.getVal(i);
                debugqueue = xQueueSend(xQueuePoti[i], &tempval, ( TickType_t ) 0);
                xQueueSend(xQueuePoti2[i], &tempval, ( TickType_t ) 0);
            }

        vTaskDelay(5);
    }
}

static void prvUartTask( void *pvParameters )
{
	(void)pvParameters;

	Usart2<>::Parameters parameters(9600);
    parameters.usart_mode = USART_Mode_Tx;
    Usart2<> usart(parameters);
    GpioB<DigitalInputFeature<GPIO_Speed_50MHz,Gpio::PUPD_UP,4> > pb;

    BaseType_t queuerecieve = pdFALSE;
	for( ;; )
    {
		uint16_t poti;
		 if (!pb[4].read())
		 {
			if (xQueuePoti2[1] != NULL)
				{
					queuerecieve = xQueueReceive(xQueuePoti2[0], &poti, (TickType_t) 0);
				}
			if(queuerecieve == pdTRUE)
			{
				auto val = normalizepoti(poti);
				if (val >= 100) val = 99;
				if (val <= -100) val = -99;
				UartSend(usart, "$P2 %ld\r", val);
			}
			vTaskDelay(50);
			if (xQueuePoti2[0] != NULL)
				{
					queuerecieve = xQueueReceive(xQueuePoti2[4], &poti, (TickType_t) 0);
				}
			if(queuerecieve == pdTRUE)
			{
				auto val = normalizepoti(poti);
				if (val > 100) val = 100;
				if (val < -100) val = -100;
				UartSend(usart, "$P0 %ld\r", val);
			}
			vTaskDelay(50);
			if (xQueuePoti2[0] != NULL)
				{
					queuerecieve = xQueueReceive(xQueuePoti2[3], &poti, (TickType_t) 0);
				}
			if(queuerecieve == pdTRUE)
			{
				int32_t val = poti;
				val = (val * 100) / 4096;
				if (val > 100) val = 100;
				UartSend(usart, "$P1 %ld\r", val);

			}
		 }
		 else
		 {
			 UartSend(usart, "$P0 0\r");
			 UartSend(usart, "$P1 0\r");
			 UartSend(usart, "$P2 0\r");
		 }



        vTaskDelay(50);
    }
}

static void prvButtonTask( void *pvParameters )
{
	(void)pvParameters;
    for (int i = 0; i < 7; i++)
        {
            xQueueButton[i] = xQueueCreate( 1, sizeof( uint16_t ) );
            configASSERT(xQueueButton[i] != NULL);
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
    BaseType_t taskcreatereturn;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    AFIO->MAPR = AFIO_MAPR_SWJ_CFG_JTAGDISABLE;

    RCC_GetClocksFreq((RCC_ClocksTypeDef*)&RCC_Clocks);

    SEGGER_SYSVIEW_Conf();
    SEGGER_SYSVIEW_Start();
    taskcreatereturn = xTaskCreate( prvDisplayTask, "Display", 256, NULL, 2, NULL );
    configASSERT(taskcreatereturn == pdPASS);
    taskcreatereturn = xTaskCreate( emwinTask, "emwin", 392, NULL, 1, NULL );
    configASSERT(taskcreatereturn == pdPASS);
    taskcreatereturn = xTaskCreate( prvStepperTask, "Stepper", 256, NULL, 9, NULL );
    configASSERT(taskcreatereturn == pdPASS);
    taskcreatereturn = xTaskCreate( prvLedTask, "Led", 392, NULL, 1, NULL );
    configASSERT(taskcreatereturn == pdPASS);
    taskcreatereturn = xTaskCreate( prvPotiTask, "Poti", 256, NULL, 5, NULL );
    configASSERT(taskcreatereturn == pdPASS);
    taskcreatereturn = xTaskCreate( prvUartTask, "Uart", 256, NULL, 5, NULL );
    configASSERT(taskcreatereturn == pdPASS);

  /* Start the scheduler. */
    vTaskStartScheduler();
  while(1)
      {
      }
  // not reached
  return 0;
}

extern "C" {
    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        signed char *pcTaskName )
    {
    	(void)xTask;
    	(void)pcTaskName;
        for(;;)
            {
                __NOP();
            }
    }
}
