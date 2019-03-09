/*
 * test-Field.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: mester
 */

#include "gtest/gtest.h"
#include <limits>

#include "Field.h"

#if 1
TEST(Field, SetField) {
	Field field(10,20);


	field.setField(Coord(1,1));
	ASSERT_EQ(1, field.getField(Coord(1,1)));
}

TEST(Field, Clear) {
	Field field(10,20);


	field.setField(Coord(1,1));
	field.ClearCompleteField();
	ASSERT_EQ(0, field.getField(Coord(1,1)));
}

TEST(Field, Sum) {
	Field field(10,20);

	field.setField(Coord(1,5));
	field.setField(Coord(3,4));
	field.setField(Coord(5,5));
	field.setField(Coord(6,1));

	ASSERT_EQ(4, field.getSumOfSetFields());

	field.ClearCompleteField();
	ASSERT_EQ(0, field.getSumOfSetFields());
}

TEST(Field, copyConstructor)
{
	Field field(10,20);
	field.setField(Coord(1,5));

	Field fieldcopy(field);
	ASSERT_EQ(1, fieldcopy.getSumOfSetFields());
}

TEST(Field, assignmentOperator)
{
	Field field(10,20);
	field.setField(Coord(1,5));

	Field fieldcopy(10,10);
	fieldcopy = field;

	Field *fieldpointer = &fieldcopy;


	ASSERT_EQ(1, fieldcopy.getSumOfSetFields());

	fieldcopy = *fieldpointer;
	ASSERT_EQ(1, fieldcopy.getSumOfSetFields());
}

TEST(Field, LinearAssignment)
{
	const uint32_t size = 10;
	Field field(size,size);
	field.setField(size*size);

	ASSERT_EQ(0, field.getSumOfSetFields());

	field.setField(size);
	ASSERT_EQ(1, field.getSumOfSetFields());
}

TEST(Field, ClearField)
{
	Field field(10,20);

	field.setField(5);
	ASSERT_EQ(1, field.getSumOfSetFields());

	field.clearField(5);
	ASSERT_EQ(0, field.getSumOfSetFields());

	field.setField(11);
	ASSERT_EQ(1, field.getSumOfSetFields());

	field.clearField(5);
	ASSERT_EQ(1, field.getSumOfSetFields());

	field.ClearCompleteField();

	field.setField(Coord(1,3));
	ASSERT_EQ(1, field.getSumOfSetFields());

	field.clearField(Coord(1,3));
	ASSERT_EQ(0, field.getSumOfSetFields());

	field.setField(Coord(2,3));
	ASSERT_EQ(1, field.getSumOfSetFields());

	field.clearField(Coord(1,3));
	ASSERT_EQ(1, field.getSumOfSetFields());

	field.ClearCompleteField();
}
/*
TEST(Field, Toobig)
{
	Field field(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());

	ASSERT_FALSE(field.isInited());
}
*/
#endif
TEST(BitField, testone)
{
	BitField bitfield(100);

	for(size_t i = 0; i < 100; i++)
	{
		ASSERT_FALSE(bitfield.getbit(i));
	}

	bitfield.setbit(47);
	ASSERT_FALSE(bitfield.getbit(46));
	ASSERT_FALSE(bitfield.getbit(48));
	ASSERT_TRUE(bitfield.getbit(47));

	bitfield.setbit(0);
	bitfield.setbit(99);
	ASSERT_TRUE(bitfield.getbit(0));
	ASSERT_TRUE(bitfield.getbit(99));

	ASSERT_FALSE(bitfield.getbit(1));
	ASSERT_FALSE(bitfield.getbit(98));

	bitfield.ClearCompleteField();
	for(size_t i = 0; i < 100; i++)
	{
		ASSERT_FALSE(bitfield.getbit(i));
	}
}
