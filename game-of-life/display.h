#pragma once

#include <stdint.h>
#include "Coord.h"


class Display {
public:
	virtual void writeNextpixel(uint32_t value) = 0;
	virtual void ClearScreen(void) = 0;
};
