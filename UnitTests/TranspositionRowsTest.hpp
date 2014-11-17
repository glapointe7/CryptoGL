#ifndef TRANSPOSITIONROWSTEST_HPP
#define TRANSPOSITIONROWSTEST_HPP

#include "Test.hpp"
#include "../src/Transposition.hpp"

namespace UnitTests
{
    class TranspositionRowsTest : public Test
    {
    protected:
       TranspositionCompleteRows *TR;

       void setUp() override
       {
          TR = new TranspositionCompleteRows({7, 1, 4, 6, 2, 5, 3, 0});
       }

       void tearDown() override
       {
          delete TR;
       }
    };

    TEST(TranspositionRowsEncode, TranspositionRowsTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare(StringTest("LATILNUSTLRIPAAINUUOECTQRALPIASNTGMARMAONORENIOITEBEEJOTUUEORBLSGDDIEOSTXSXXXXXT"), TR->encode(clear_text));
    }

    TEST(TranspositionRowsDecode, TranspositionRowsTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSXXXXXX";

       compare(clear_text, TR->decode("LATILNUSTLRIPAAINUUOECTQRALPIASNTGMARMAONORENIOITEBEEJOTUUEORBLSGDDIEOSTXSXXXXXT"));
    }
}

#endif