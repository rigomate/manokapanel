#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "Coord.h"

class Field{
public:
	Field(uint32_t xwidth, uint32_t ywidth) : xwidth(xwidth), ywidth(ywidth) {
		FieldSize = xwidth * ywidth;
		FieldArray = new uint32_t[FieldSize];
		ClearCompleteField();
	}
	~Field()
	{
		delete [] FieldArray;
	}
	Field(const Field& source);
	Field& operator=(const Field & source);

	uint32_t getField(Coord &&coord);
	uint32_t getField(Coord &coord);
	uint32_t getField(uint32_t LinearAddress);

	uint32_t getSumOfSetFields(void);
	void setField(Coord &&coord);
	void setField(Coord &coord);
	void setField(uint32_t LinearAddress);

	void clearField(Coord &&coord);
	void clearField(Coord &coord);
	void clearField(uint32_t LinearAddress);

	bool isInited();


	void ClearCompleteField(void);

	uint32_t getXWidth(void) {return xwidth;}
	uint32_t getYWidth(void) {return ywidth;}

private:
	uint32_t *FieldArray;
	uint32_t FieldSize;
	uint32_t xwidth;
	uint32_t ywidth;

};
