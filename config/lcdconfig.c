/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2009  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/
#include <stdint.h>
#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "FreeRTOS.h"
#include "semphr.h"

/*********************************************************************
*
*       Layer configuration
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS 128
#define YSIZE_PHYS 32
#define BITSPERPIXEL 1

#define NUM_BUFFERS 2

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_1

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_LIN_1

//
// Orientation
//
//#define DISPLAY_ORIENTATION (0)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
#define DISPLAY_ORIENTATION (GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_Y | GUI_SWAP_XY)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y | GUI_SWAP_XY)

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif
#ifndef   DISPLAY_ORIENTATION
  #define DISPLAY_ORIENTATION 0
#endif

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

volatile uint8_t videoRAM[((XSIZE_PHYS * YSIZE_PHYS) / 8 ) * NUM_BUFFERS];
unsigned long Addr;

/*********************************************************************
*
*       _InitController
*
* Purpose:
*   Initializes the display controller
*/
static void _InitController(void) {
	//xpt2046Init();
	//TM_ILI9341_Init();
}

#if 0
/*********************************************************************
*
*       _ReadPixel
*/
static U16 _ReadPixel(int LayerIndex) {
  #ifndef WIN32
  U16 aData[3];
  U16 Index;

  GUI_USE_PARA(LayerIndex);
  //
  // Switch to read mode
  //
  LCD_X_8080_16_Write00(0x2E);
  //
  // Dummy- and data read
  //
  LCD_X_8080_16_ReadM01(aData, GUI_COUNTOF(aData));
  //
  // Convert to index
  //
  Index = (aData[2] >> 11) | ((aData[1] & 0xfc) << 3) | (aData[1] & 0xf800);
  #else
  U16 Index;

  GUI_USE_PARA(LayerIndex);
  Index = 0;
  #endif
  return Index;
}

/*********************************************************************
*
*       _ReadMPixels
*/
static void _ReadMPixels(int LayerIndex, U16 * pBuffer, U32 NumPixels) {
  #ifndef WIN32
  U16 Data;
  U16 Index;
  int State;

  GUI_USE_PARA(LayerIndex);
  //
  // Switch to read mode
  //
  LCD_X_8080_16_Write00(0x2E);
  //
  // Dummy- read
  //
  LCD_X_8080_16_Read01();
  for (State = 0; NumPixels; NumPixels--, State ^= 1) {
    switch (State) {
    case 0:
      Data = LCD_X_8080_16_Read01();
      Index  =  Data & 0xf800;        // blue
      Index |= (Data & 0x00fc) <<  3; // green
      Data = LCD_X_8080_16_Read01();
      Index |= (Data & 0xf800) >> 11; // red
      break;
    case 1:
      Index  = (Data & 0x00f8) <<  8; // blue
      Data = LCD_X_8080_16_Read01();
      Index |= (Data & 0xfc00) >>  5; // green
      Index |= (Data & 0x00f8) >>  3; // red
      break;
    }
    *pBuffer++ = Index;
  }
  #else
  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(pBuffer);
  GUI_USE_PARA(NumPixels);
  #endif
  return;
}
#endif
/*********************************************************************
*
*       Public code
*
**********************************************************************
*/


/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  GUI_PORT_API PortAPI = {0};
  //CONFIG_FLEXCOLOR Config = {0};


  GUI_MULTIBUF_Config(NUM_BUFFERS);

  //
  // Set display driver and color conversion for 1st layer
  //
  pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration
  //
  if (DISPLAY_ORIENTATION & GUI_SWAP_XY) {
    LCD_SetSizeEx (0, YSIZE_PHYS,   XSIZE_PHYS);
    LCD_SetVSizeEx(0, VYSIZE_PHYS,  VXSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS,   YSIZE_PHYS);
    LCD_SetVSizeEx(0, VXSIZE_PHYS,  VYSIZE_PHYS);
  }

  LCD_SetVRAMAddrEx(0, (void *)videoRAM);
  Addr = videoRAM;
  //
  // Function selection, hardware routines (PortAPI) and operation mode (bus, bpp and cache)
  //
  /*
  PortAPI.pfWrite8_A0  = TM_ILI9341_SendCommand;
  PortAPI.pfWrite8_A1  = TM_ILI9341_SendData;
  PortAPI.pfWriteM8_A1 = TM_ILI9341_SendDataMultiple;
  PortAPI.pfRead8_A1   = TM_ILI9341_ReadData;
  PortAPI.pfReadM8_A1  = TM_ILI9341_ReadDataMulitple;
*/



  //GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B8);
  //
  // Orientation
  //
  //Config.Orientation = DISPLAY_ORIENTATION;
  //GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set custom reading function(s)
  //

  // Mate: These should be just speed up operations, so they are not necessary
  //LCD_SetDevFunc(0, LCD_DEVFUNC_READPIXEL,   (void(*)(void))_ReadPixel);
  //LCD_SetDevFunc(0, LCD_DEVFUNC_READMPIXELS, (void(*)(void))_ReadMPixels);
}


int _PendingBuffer;
static void _ISR_EndOfFrame(void) {
    unsigned long BufferSize;
    if (_PendingBuffer >= 0) {
    //
    // Calculate address of the given buffer
    //
    BufferSize = (XSIZE_PHYS * YSIZE_PHYS * BITSPERPIXEL) / 8;
    //Addr = videoRAM + BufferSize * pData->Index;
    //
    // Make the given buffer visible
    //
    //AT91C_LCDC_BA1 = Addr;
    //
    // Send a confirmation that the buffer is visible now
    //
    GUI_MULTIBUF_Confirm(_PendingBuffer);
    _PendingBuffer = -1;
    }
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*/
extern SemaphoreHandle_t xSemaphore;
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  LCD_X_SHOWBUFFER_INFO * pbuffer;
  unsigned long BufferSize;

  GUI_USE_PARA(LayerIndex);
  GUI_USE_PARA(pData);
  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    _InitController();
    return 0;
  }
  case LCD_X_SHOWBUFFER:
      pbuffer = (LCD_X_SHOWBUFFER_INFO *)pData;
      BufferSize = (XSIZE_PHYS * YSIZE_PHYS * BITSPERPIXEL) / 8;
      Addr = videoRAM + BufferSize * pbuffer->Index;
      //
      // Remember buffer index to be used by ISR
      //
      if (xSemaphore != NULL)
      {
          if( xSemaphoreGive( xSemaphore ) != pdTRUE )
          {
              // We would not expect this call to fail because we must have
              // obtained the semaphore to get here.
          }
      }
      GUI_MULTIBUF_Confirm(pbuffer->Index);
      break;
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

void NMI_Handler(void)
{
    while(1)
    {
        asm("nop");
    }
}
void HardFault_Handler(void)
{

        asm("nop");
}
void MemManage_Handler(void)
{
    while(1)
    {
        asm("nop");
    }
}
void BusFault_Handler(void)
{
    while(1)
    {
        asm("nop");
    }
}
void UsageFault_Handler(void)
{
    while(1)
    {
        asm("nop");
    }
}
