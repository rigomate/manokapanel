#pragma once

class Coord {

#include <stdint.h>

public:
	Coord(uint32_t x, uint32_t y) : x(x), y(y) {};
	virtual uint32_t getx(void) { return x; }
	virtual uint32_t gety(void) { return y; }

private:
	uint32_t x = 0;
	uint32_t y = 0;
};
