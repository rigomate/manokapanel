/*
 * emwin-display.h
 *
 *  Created on: Mar 7, 2019
 *      Author: mester
 */

#pragma once

#include "display.h"

class EmwinDisplay : public Display{

public:

	EmwinDisplay(uint32_t xwidth, uint32_t ywidth) : xwidth(xwidth), ywidth(ywidth) {
		ActualPixel = 0;
		CurrentRow = 0;
	};

	void writeNextpixel(uint32_t value) override;
	void ClearScreen(void) override;
	uint32_t getXwidth() { return xwidth; }
	uint32_t getYwidth() { return ywidth; }

private:
	uint32_t xwidth;
	uint32_t ywidth;
	uint32_t ActualPixel;
	uint32_t CurrentRow;
};
