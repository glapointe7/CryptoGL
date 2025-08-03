#pragma once

#include "Test.hpp"
#include "../src/LRC.hpp"

namespace UnitTests
{
    class LRCTest : public Test
    {
    protected:
       LRC *S;
       void setUp() override
       {
          S = new LRC();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(LRCEncodeNormalText, LRCTest)
    {
       const BytesVector message = {0x41, 0x30, 0x31, 0x54, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
          0x30, 0x30, 0x31, 0x30, 0x39, 0x39, 0x34, 0x35, 0x30, 0x30, 0x03};

       compare(static_cast<uint8_t>(0x27), S->encode(message));
    }
}