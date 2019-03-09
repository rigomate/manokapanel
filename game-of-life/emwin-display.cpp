/*
 * emwin-display.cpp
 *
 *  Created on: Mar 7, 2019
 *      Author: mester
 */

#include "emwin-display.h"
extern "C" {
#include "GUI.h"
}

void EmwinDisplay::writeNextpixel(uint32_t value)
{

	if ( ((ActualPixel % xwidth) == 0) && (ActualPixel > 0))
	{
		CurrentRow++;
	}

	if (value == 1)
	{
		volatile uint32_t xCoord = ActualPixel % xwidth;
		GUI_DrawPixel(xCoord, CurrentRow);
	}

		ActualPixel++;
}

void EmwinDisplay::ClearScreen(void)
{
	ActualPixel = 0;
	CurrentRow = 0;

	GUI_Clear();
}
