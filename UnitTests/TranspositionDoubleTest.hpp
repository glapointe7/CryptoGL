#pragma once

#include "Test.hpp"
#include "../src/Transposition.hpp"

namespace UnitTests
{
    class TranspositionDoubleTest : public Test
    {
    protected:
       TranspositionDouble *TD;

       void setUp() override
       {
          TD = new TranspositionDouble({2, 3, 7, 1, 6, 5, 0, 4}, {1, 6, 0, 7, 8, 4, 3, 5, 2});
       }

       void tearDown() override
       {
          delete TD;
       }
    };

    TEST(TranspositionDoubleEncode, TranspositionDoubleTest)
    {
       const StringTest clear_text = "YOURMOTHERWASAHAMSTERANDYOURFATHERSMELTOFELDERBERRIES";

       compare("NDODRWTRFHASEERAERMROFLBEOERSAYEAEIHMRALUTERHMTTYSOSU", TD->encode(clear_text));
    }

    TEST(TranspositionDoubleDecode, TranspositionDoubleTest)
    {
       const StringTest clear_text = "YOURMOTHERWASAHAMSTERANDYOURFATHERSMELTOFELDERBERRIES";

       compare(clear_text, TD->decode("NDODRWTRFHASEERAERMROFLBEOERSAYEAEIHMRALUTERHMTTYSOSU"));
    }
}