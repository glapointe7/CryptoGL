
#ifndef SKIPJACKTEST_HPP
#define SKIPJACKTEST_HPP

#include "Test.hpp"
#include "../src/Skipjack.hpp"

namespace UnitTests
{
    class SkipjackTest : public Test
    {
    protected:
       Skipjack *A, *B;

       void setUp() override
       {
          A = new Skipjack({0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11});
          B = new Skipjack({0xf8, 0xda, 0x02, 0x64, 0x77, 0x22, 0xf7, 0x10, 0x3a, 0xdf});
       }

       void tearDown() override
       {
          delete A;
          delete B;
       }
    };

    TEST(SkipjackEncodeFirst, SkipjackTest)
    {
       const BytesVector clear_text = {0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa};

       compare("2587CAE27A12D300", A->encode(clear_text));
    }

    TEST(SkipjackDecodeFirst, SkipjackTest)
    {
       const BytesVector clear_text = {0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa};
       const StringTest message("2587CAE27A12D300");

       compare(clear_text, A->decode(message.hexToBytes()));
    }

    TEST(SkipjackEncodeSecond, SkipjackTest)
    {
       const BytesVector clear_text = {0x1d, 0xdf, 0x39, 0xab, 0xf5, 0xcd, 0x71, 0x1e};

       compare("C92D22324C6B31AE", B->encode(clear_text));
    }

    TEST(SkipjackDecodeSecond, SkipjackTest)
    {
       const BytesVector clear_text = {0x1d, 0xdf, 0x39, 0xab, 0xf5, 0xcd, 0x71, 0x1e};
       const StringTest message("C92D22324C6B31AE");

       compare(clear_text, B->decode(message.hexToBytes()));
    }
}

#endif