#pragma once

#include "Test.hpp"
#include "../src/TwoSquares.hpp"

namespace UnitTests
{
    class TwoSquaresTest : public Test
    {
    protected:
       TwoSquares *P;

       void setUp() override
       {
          P = new TwoSquares("ROMEO", "JULIETTE");
       }

       void tearDown() override
       {
          delete P;
       }
    };

    TEST(TwoSquaresEncode, TwoSquaresTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare("OFGANTHOHOOCUBFCOMJFYBJPOBFVGGRPUIUBEJJGGPONIRJSRAUSTRJFOELRLFUORGJYAQGINZ", P->encode(clear_text));
    }

    TEST(TwoSquaresDecode, TwoSquaresTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, P->decode("OFGANTHOHOOCUBFCOMJFYBJPOBFVGGRPUIUBEJJGGPONIRJSRAUSTRJFOELRLFUORGJYAQGINZ"));
    }
}