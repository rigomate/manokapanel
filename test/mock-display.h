#pragma once

#include "gmock/gmock.h"
#include "display.h"

class MockCoord : public Coord {
 public:
  MOCK_METHOD0(getx,
      uint32_t(void));
  MOCK_METHOD0(gety,
      uint32_t(void));
};

class MockDisplay : public Display {
 public:
  MOCK_METHOD1(writeNextpixel,
      void(uint32_t value));
  MOCK_METHOD0(ClearScreen,
      void(void));
};
