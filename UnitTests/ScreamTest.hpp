
#ifndef SCREAMTEST_HPP
#define SCREAMTEST_HPP

#include "Test.hpp"
#include "../src/Scream.hpp"

namespace UnitTests
{
    class ScreamTest : public Test
    {
    protected:
       Scream_S *S;

       void setUp() override
       {
          S = new Scream_S({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10},
                         BytesVector(16, 0));
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(ScreamGenerate, ScreamTest)
    {
       const BytesVector keystream = S->generateKeystream();
       const BytesVector input = keystream.range(0, 16);

       compare("748C59F20D769EA87A6DC18746E64AC0", input.toHexString());
    }
}

#endif