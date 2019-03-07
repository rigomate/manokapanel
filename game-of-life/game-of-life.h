#pragma once

#include <stdint.h>
#include "display.h"
#include "Field.h"
//#include <vector>

class GameOfLife {

public:

	enum FieldInit
	{
		RANDOM,
		COOL
	};

	GameOfLife(Display *display, uint32_t xwidth, uint32_t ywidth, FieldInit fieldinit) : display(display), xwidth(xwidth), ywidth(ywidth){
		field = new Field(xwidth, ywidth);
		FieldSize = xwidth * ywidth;
		
		ClearField();

		if (fieldinit == RANDOM)
		{
			SetRandom();
		}
		else if (fieldinit == COOL)
		{
			SetCool();
		}
		
	}/*
	GameOfLife(Display *display, uint32_t xwidth, uint32_t ywidth, std::vector<Coord> coords) : display(display), xwidth(xwidth), ywidth(ywidth){
		field = new Field(xwidth, ywidth);
		FieldSize = xwidth * ywidth;

		ClearField();
		for (auto i : coords)
			setFieldValue(i, field);
	}
*/
	void DrawTable(void);
	void NextStep(void);
	uint32_t GetSumOfLiving(void);
	uint32_t getFieldValue(Coord &&coord);
	void ClearField(void);
	uint32_t SumNeighbours(Coord &&coord);

private:
	Display *display;
	uint32_t xwidth;
	uint32_t ywidth;
	uint32_t FieldSize;
	Field *field;

	void SetRandom(void);
	void SetCool(void);

	void setFieldValue(Coord &&coord, Field *FieldToSet);
	void setFieldValue(Coord &coord, Field *FieldToSet);
	void clearFieldValue(Coord &&coord, Field *FieldToClear);

};
