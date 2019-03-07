/*
 * Field.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: mester
 */


#include "Field.h"

#include <cstring>

// Copy constructor
Field::Field(const Field& source)
{
	xwidth = source.xwidth;
	ywidth = source.ywidth;
	FieldSize = source.FieldSize;

    // FieldArray is a pointer, so we need to deep copy it if it is non-null
    if (source.FieldArray)
    {
        // allocate memory for our copy
    	FieldArray = new uint32_t[FieldSize];

        // do the copy
        for (uint32_t i=0; i < FieldSize; ++i)
            FieldArray[i] = source.FieldArray[i];
    }
    else
    	FieldArray = nullptr;
}

// Assignment operator
Field& Field::operator=(const Field & source)
{
    // check for self-assignment
    if (this == &source)
        return *this;

    // first we need to deallocate any dynamic values
    delete[] FieldArray;

	xwidth = source.xwidth;
	ywidth = source.ywidth;
	FieldSize = source.FieldSize;

    // FieldArray is a pointer, so we need to deep copy it if it is non-null
    if (source.FieldArray)
    {
        // allocate memory for our copy
    	FieldArray = new uint32_t[FieldSize];

        // do the copy
        for (uint32_t i=0; i < FieldSize; ++i)
            FieldArray[i] = source.FieldArray[i];
    }
    else
    	FieldArray = nullptr;

    return *this;
}

void Field::setField(Coord &coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
	FieldArray[coord.getx() + (coord.gety()*xwidth)] = 1;
	}
}

void Field::setField(Coord &&coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
	FieldArray[coord.getx() + (coord.gety()*xwidth)] = 1;
	}
}

void Field::setField(uint32_t LinearAddress)
{
	if (LinearAddress < FieldSize)
	{
		FieldArray[LinearAddress] = 1;
	}
}

void Field::clearField(Coord &coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
	FieldArray[coord.getx() + (coord.gety()*xwidth)] = 0;
	}
}

void Field::clearField(Coord &&coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
	FieldArray[coord.getx() + (coord.gety()*xwidth)] = 0;
	}
}

void Field::clearField(uint32_t LinearAddress)
{
	if (LinearAddress < FieldSize)
	{
	FieldArray[LinearAddress] = 0;
	}
}

bool Field::isInited()
{
	if (FieldArray != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint32_t Field::getField(Coord &coord)
{
	return FieldArray[coord.getx() + (coord.gety()*xwidth)];
}

uint32_t Field::getField(Coord &&coord)
{
	return FieldArray[coord.getx() + (coord.gety()*xwidth)];
}

uint32_t Field::getField(uint32_t LinearAddress)
{
	return FieldArray[LinearAddress];
}

void Field::ClearCompleteField(void)
{
	std::memset(FieldArray, 0, sizeof(*FieldArray) * xwidth * ywidth);
}

uint32_t Field::getSumOfSetFields(void)
{
	uint32_t Sum = 0;
	for (uint32_t y = 0; y < ywidth; y++)
	{
		for (uint32_t x = 0; x < xwidth; x++)
		{
			Sum += getField(Coord(x, y));
		}
	}
	return Sum;
}
