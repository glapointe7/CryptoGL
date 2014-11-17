#ifndef TRANSPOSITIONCOLUMNSTEST_HPP
#define TRANSPOSITIONCOLUMNSTEST_HPP

#include "Test.hpp"
#include "../src/Transposition.hpp"

namespace UnitTests
{
    class TranspositionColumnsTest : public Test
    {
    protected:
       TranspositionCompleteColumns *TC;

       void setUp() override
       {
          TC = new TranspositionCompleteColumns({1u, 4u, 0u, 2u, 3u});
       }

       void tearDown() override
       {
          delete TC;
       }
    };

    TEST(TranspositionColumnsEncode, TranspositionColumnsTest)
    {
       const StringTest clear_text = "SALUTLESPETITSPOTS";

       compare(StringTest("LSTSSLTOUPSXTEPXAEIT"), TC->encode(clear_text));
    }

    TEST(TranspositionColumnsDecode, TranspositionColumnsTest)
    {
       const StringTest clear_text = "SALUTLESPETITSPOTSXX";

       compare(clear_text, TC->decode("LSTSSLTOUPSXTEPXAEIT"));
    }
}

#endif