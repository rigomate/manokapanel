

#include "game-of-life.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <cstring>
#include "timer/manotimer.h"
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

void GameOfLife::ClearField(void)
{
	field->ClearCompleteField();
}

void GameOfLife::SetRandom(void)
{
	int random_integer;
	volatile int  randomint;
	//srand((unsigned)time(0));
	MillisecondTimerMano Timer;
	volatile int Seedval = static_cast<unsigned>(Timer.millis());
	srand(Seedval);
	

	int lowest = 0, highest = 1;
	int range = (highest - lowest) + 1;
	for (uint32_t i = 0; i < xwidth * ywidth; i++) {
		randomint = rand();


		if (CHECK_BIT(randomint, 0))
		{
			field->setField(i);
		}
		else
		{
			field->clearField(i);
		}
	}
}

void GameOfLife::SetCool(void)
{
	setFieldValue(Coord(1, 1), field);
	setFieldValue(Coord(2, 1), field);
	setFieldValue(Coord(3, 1), field);
}

void GameOfLife::DrawTable(void)
{
	display->ClearScreen();

	for (uint32_t i = 0; i < FieldSize; i++)
	{
		display->writeNextpixel(field->getField(i));
	}
}

void GameOfLife::NextStep(void)
{
	Field TempField(*field);


	for (uint32_t y = 0; y < ywidth; y++)
	{
		for (uint32_t x = 0; x < xwidth; x++)
		{
			uint32_t Sum = SumNeighbours(Coord(x, y));
			
			if (getFieldValue(Coord(x, y)) == 1)
			{
				if (Sum < 2)
				{
					//underpopulation
					clearFieldValue(Coord(x, y), &TempField);
				}
				else if (Sum <= 3)
				{
					//just live
					setFieldValue(Coord(x, y), &TempField);
				}
				else
				{
					//overpopulation
					clearFieldValue(Coord(x, y), &TempField);
				}
			}
			else
			{
				if (Sum == 3)
				{
					//reproduction
					setFieldValue(Coord(x, y), &TempField);
				}
			}
		}
	}
	*field = TempField;
}

uint32_t GameOfLife::GetSumOfLiving(void)
{
	uint32_t Sum = 0;
	for (uint32_t y = 0; y < ywidth; y++)
	{
		for (uint32_t x = 0; x < xwidth; x++)
		{
			Sum += getFieldValue(Coord(x, y));
		}
	}
	return Sum;
}

uint32_t GameOfLife::getFieldValue(Coord &&coord)
{
	return field->getField(coord);
}

void GameOfLife::setFieldValue(Coord &&coord, Field *FieldToSet)
{
	FieldToSet->setField(coord);
}

void GameOfLife::setFieldValue(Coord &coord, Field *FieldToSet)
{
	FieldToSet->setField(coord);
}

void GameOfLife::clearFieldValue(Coord &&coord, Field *FieldToClear)
{
	FieldToClear->clearField(coord);
}

uint32_t GameOfLife::SumNeighbours(Coord &&coord)
{
	uint32_t returnval = 0;
	for (int32_t y = static_cast<int32_t>(coord.gety()) - 1; y <= static_cast<int32_t>(coord.gety()) + 1; y++)
	{
		for (int32_t x = static_cast<int32_t>(coord.getx()) - 1; x <= static_cast<int32_t>(coord.getx()) + 1; x++)
		{
			//Boundary checks of the field
			if (x < 0) continue;
			if (y < 0) continue;

			if (x >= static_cast<int32_t>(xwidth)) continue;
			if (y >= static_cast<int32_t>(ywidth)) continue;

			//Same value
			if (x == static_cast<int32_t>(coord.getx()) && y == static_cast<int32_t>(coord.gety()))
			{
				continue;
			}
			if (getFieldValue(Coord(x, y)) == 1)
			{
				returnval++;
			}
		}
	}

	return returnval;
}
