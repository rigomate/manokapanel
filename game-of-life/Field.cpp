/*
 * Field.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: mester
 */


#include "Field.h"

#include <cstring>

// Copy constructor
BitField::BitField(const BitField& source)
{
	FieldElements = source.FieldElements;

    // FieldArray is a pointer, so we need to deep copy it if it is non-null
    if (source.FieldArray)
    {
        // allocate memory for our copy
    	FieldArray = new uint8_t[FieldElements];

        // do the copy
        for (uint32_t i=0; i < FieldElements; ++i)
            FieldArray[i] = source.FieldArray[i];
    }
    else
    	FieldArray = nullptr;
}

// Assignment operator
BitField& BitField::operator=(const BitField & source)
{
    // check for self-assignment
    if (this == &source)
        return *this;

    // first we need to deallocate any dynamic values
    delete[] FieldArray;

    FieldElements = source.FieldElements;

    // FieldArray is a pointer, so we need to deep copy it if it is non-null
    if (source.FieldArray)
    {
        // allocate memory for our copy
    	FieldArray = new uint8_t[FieldElements];

        // do the copy
        for (uint32_t i=0; i < FieldElements; ++i)
            FieldArray[i] = source.FieldArray[i];
    }
    else
    	FieldArray = nullptr;

    return *this;
}

uint32_t BitField::getnumberofbyte(uint32_t bitnum)
{
	return bitnum / 8;
}

uint32_t BitField::getnumberofbit(uint32_t bitnum)
{
	return bitnum % 8;
}

void BitField::setbit(uint32_t bitnum)
{
	uint8_t tempval = FieldArray[getnumberofbyte(bitnum)];

	tempval |= (1 << getnumberofbit(bitnum));
	FieldArray[getnumberofbyte(bitnum)] = tempval;
}
void BitField::clearbit(uint32_t bitnum)
{
	uint8_t tempval = FieldArray[getnumberofbyte(bitnum)];

	tempval &= ~(1 << getnumberofbit(bitnum));
	FieldArray[getnumberofbyte(bitnum)] = tempval;
}
bool BitField::getbit(uint32_t bitnum)
{
	uint8_t tempval = FieldArray[getnumberofbyte(bitnum)];

	if (tempval & (1 << getnumberofbit(bitnum)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BitField::ClearCompleteField(void)
{
	std::memset(FieldArray, 0, sizeof(*FieldArray) * FieldElements);
}



#if 1
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
    	FieldArray = new BitField(FieldSize);

        // do the copy
            *FieldArray = *source.FieldArray;
    }
    else
    	FieldArray = nullptr;
}
#endif
// Assignment operator
Field& Field::operator=(const Field & source)
{
    // check for self-assignment
    if (this == &source)
        return *this;

    // first we need to deallocate any dynamic values
    delete FieldArray;

	xwidth = source.xwidth;
	ywidth = source.ywidth;
	FieldSize = source.FieldSize;

    // FieldArray is a pointer, so we need to deep copy it if it is non-null
    if (source.FieldArray)
    {
        // allocate memory for our copy
		FieldArray = new BitField(FieldSize);

        // do the copy
        for (uint32_t i=0; i < FieldSize; ++i)
        {
        	if (source.FieldArray->getbit(i))
        	{
        		FieldArray->setbit(i);
        	}
        }

    }
    else
    	FieldArray = nullptr;

    return *this;
}

void Field::setField(Coord &coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
		FieldArray->setbit(coord.getx() + (coord.gety()*xwidth));
	}
}

void Field::setField(Coord &&coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
		FieldArray->setbit(coord.getx() + (coord.gety()*xwidth));
	}
}

void Field::setField(uint32_t LinearAddress)
{
	if (LinearAddress < FieldSize)
	{
		FieldArray->setbit(LinearAddress);
	}
}

void Field::clearField(Coord &coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
	FieldArray->clearbit(coord.getx() + (coord.gety()*xwidth));
	}
}

void Field::clearField(Coord &&coord)
{
	if (coord.getx() < xwidth && coord.gety() < ywidth)
	{
	FieldArray->clearbit(coord.getx() + (coord.gety()*xwidth));
	}
}

void Field::clearField(uint32_t LinearAddress)
{
	if (LinearAddress < FieldSize)
	{
		FieldArray->clearbit(LinearAddress);
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
	return FieldArray->getbit(coord.getx() + (coord.gety()*xwidth));
}

uint32_t Field::getField(Coord &&coord)
{
	return FieldArray->getbit(coord.getx() + (coord.gety()*xwidth));
}

uint32_t Field::getField(uint32_t LinearAddress)
{
	return FieldArray->getbit(LinearAddress);
}

void Field::ClearCompleteField(void)
{
	FieldArray->ClearCompleteField();
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
