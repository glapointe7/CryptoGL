/*
 * Source : https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/idea/Idea-128-64.verified.test-vectors
 */
#ifndef IDEATEST_HPP
#define IDEATEST_HPP

#include "Test.hpp"
#include "../src/IDEA.hpp"

namespace UnitTests
{
    class IDEATest : public Test
    {
    protected:
       IDEA *A, *B;

       void setUp() override
       {
          A = new IDEA({0x9D, 0x40, 0x75, 0xC1, 0x03, 0xBC, 0x32, 0x2A, 0xFB, 0x03, 0xE7, 0xBE, 0x6A, 0xB3, 0x00, 0x06});
          B = new IDEA(BytesVector(16, 0));
       }

       void tearDown() override
       {
          delete A;
          delete B;
       }
    };

    TEST(IDEAEncodeEightValues, IDEATest)
    {
       const BytesVector clear_text = {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};

       compare("F5DB1AC45E5EF9F9", A->encode(clear_text));
    }

    TEST(IDEADecodeEightValues, IDEATest)
    {
       const BytesVector clear_text = {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};
       const StringTest message("F5DB1AC45E5EF9F9");
       compare(clear_text, A->decode(message.hexToBytes()));
    }

    TEST(IDEAEncodeZeroValues, IDEATest)
    {
       const BytesVector clear_text = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

       compare("0001000100000000", B->encode(clear_text));
    }

    TEST(IDEADecodeZeroValues, IDEATest)
    {
       const BytesVector clear_text(8, 0);
       const StringTest message("0001000100000000");
       compare(clear_text, B->decode(message.hexToBytes()));
    }
}

#endif