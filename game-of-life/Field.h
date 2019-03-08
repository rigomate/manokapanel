#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "Coord.h"

class BitField {
public:
	BitField(uint32_t bits) {
		FieldElements = (bits / 8) + 1;
		FieldArray = new uint8_t[FieldElements];
		ClearCompleteField();
	}
	~BitField(){
		delete[] FieldArray;
	}

	BitField(const BitField& source);
	BitField& operator=(const BitField & source);

	void setbit(uint32_t bitnum);
	void clearbit(uint32_t bitnum);
	bool getbit(uint32_t bitnum);
	void ClearCompleteField(void);

private:
	uint32_t FieldElements;
	uint8_t *FieldArray;

	uint32_t getnumberofbyte(uint32_t bitnum);
	uint32_t getnumberofbit(uint32_t bitnum);
};

class Field{
public:
	Field(uint32_t xwidth, uint32_t ywidth) : xwidth(xwidth), ywidth(ywidth) {
		FieldSize = xwidth * ywidth;
		FieldArray = new BitField(FieldSize);
		ClearCompleteField();
	}
	~Field()
	{
		delete FieldArray;
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
	BitField *FieldArray;
	uint32_t FieldSize;
	uint32_t xwidth;
	uint32_t ywidth;

};
