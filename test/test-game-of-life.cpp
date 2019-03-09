/*
 * test-game-of-life.cpp
 *
 *  Created on: Mar 4, 2019
 *      Author: mester
 */


#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "game-of-life.h"
#include "mock-display.h"

#include <vector>

using namespace testing;
using testing::AtLeast;

TEST(GameOfLive, ClearDisplay)
{
	 MockDisplay mockdisplay;

	EXPECT_CALL(mockdisplay, ClearScreen())
	 .Times(AtLeast(1));

	GameOfLife gameoflife(&mockdisplay, 30, 30, GameOfLife::COOL);
	gameoflife.DrawTable();
}

TEST(GameOfLife, SumOfLiving) {
    MockDisplay mockdisplay;

    GameOfLife gameoflife(&mockdisplay, 30, 30, GameOfLife::COOL);

	ASSERT_EQ(3, gameoflife.GetSumOfLiving());

	gameoflife.NextStep();
	ASSERT_EQ(3, gameoflife.GetSumOfLiving());
}

TEST(GameOfLife, SumOfNeighbours) {
    MockDisplay mockdisplay;

    GameOfLife gameoflife(&mockdisplay, 30, 30, GameOfLife::COOL);

	ASSERT_EQ(3, gameoflife.SumNeighbours(Coord(2,0)));
	ASSERT_EQ(1, gameoflife.SumNeighbours(Coord(1,1)));
	ASSERT_EQ(2, gameoflife.SumNeighbours(Coord(1,0)));

}

TEST(GameOfLife, SumOfNeighboursNotOnSquare) {
    MockDisplay mockdisplay;

    GameOfLife gameoflife(&mockdisplay, 30, 40, GameOfLife::COOL);

	ASSERT_EQ(3, gameoflife.SumNeighbours(Coord(2,0)));
	ASSERT_EQ(1, gameoflife.SumNeighbours(Coord(1,1)));
	ASSERT_EQ(2, gameoflife.SumNeighbours(Coord(1,0)));

}

TEST(GameOfLife, CalcStep) {
    MockDisplay mockdisplay;

    GameOfLife gameoflife(&mockdisplay, 30, 30, GameOfLife::COOL);

    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(1,1)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,1)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(3,1)));

    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(2,0)));
    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(2,2)));

	gameoflife.NextStep();
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,0)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,1)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,2)));

    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(1,1)));
    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(3,1)));
}

TEST(GameOfLife, CalcStepNotonSquare) {
    MockDisplay mockdisplay;

    GameOfLife gameoflife(&mockdisplay, 30, 40, GameOfLife::COOL);

    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(1,1)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,1)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(3,1)));

    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(2,0)));
    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(2,2)));

	gameoflife.NextStep();
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,0)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,1)));
    ASSERT_EQ(1, gameoflife.getFieldValue(Coord(2,2)));

    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(1,1)));
    ASSERT_EQ(0, gameoflife.getFieldValue(Coord(3,1)));
}




TEST(GameOfLife, Overpopulation) {
    MockDisplay mockdisplay;

    std::vector<Coord> coords = {Coord(0,0), Coord(1,0), Coord(2,0), Coord(0,1), Coord(1,1), Coord(2,1)};

    GameOfLife gameoflife(&mockdisplay, 30, 30, coords);
    ASSERT_EQ(6, gameoflife.GetSumOfLiving());

	gameoflife.NextStep();

	ASSERT_EQ(5, gameoflife.GetSumOfLiving());

}

TEST(GameOfLife, BottomRightCornerSum) {
    MockDisplay mockdisplay;

    std::vector<Coord> coords = {Coord(9,9), Coord(9,8), Coord(8,9), Coord(8,8)};

    GameOfLife gameoflife(&mockdisplay, 10, 10, coords);
    ASSERT_EQ(4, gameoflife.GetSumOfLiving());

	ASSERT_EQ(3, gameoflife.SumNeighbours(Coord(9,9)));

}

TEST(GameOfLife, BottomRightCornerStep) {
    MockDisplay mockdisplay;

    std::vector<Coord> coords = {Coord(9,9), Coord(9,8), Coord(9,7)};

    GameOfLife gameoflife(&mockdisplay, 10, 10, coords);
    ASSERT_EQ(3, gameoflife.GetSumOfLiving());

    gameoflife.NextStep();
    ASSERT_EQ(2, gameoflife.GetSumOfLiving());

    gameoflife.NextStep();
    ASSERT_EQ(0, gameoflife.GetSumOfLiving());
}

TEST(GameOfLife, Bottom) {
    MockDisplay mockdisplay;

    std::vector<Coord> coords = {Coord(5,31), Coord(6,31)};

    GameOfLife gameoflife(&mockdisplay, 128, 32, coords);
    ASSERT_EQ(2, gameoflife.GetSumOfLiving());

    gameoflife.NextStep();
    ASSERT_EQ(0, gameoflife.GetSumOfLiving());
}

