
#ifndef VIGENEREMULTTEST_HPP
#define	VIGENEREMULTTEST_HPP

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class VigenereMultTest : public Test
    {
    protected:
       VigenereMult *V;

       void setUp() override
       {
          V = new VigenereMult("TABARNAK");
       }

       void tearDown() override
       {
          delete V;
       }
    };

    TEST(VigenereMultEncode, VigenereMultTest) 
    {
      const StringTest clear_text = "TIGARSESTENBAWATARNACHEONDIRAIT";

      compare(StringTest("400 9 14 1 324 266 5 209 400 5 28 2 18 322 1 220 20 18 28 1 54 112 5 165 280 4 18 18 18 126 20 "), 
              V->encode(clear_text));
    }

    TEST(VigenereMultDecode, VigenereMultTest) 
    {
      const StringTest clear_text = "TIGARSESTENBAWATARNACHEONDIRAIT";

      compare(clear_text, 
              V->decode("400 9 14 1 324 266 5 209 400 5 28 2 18 322 1 220 20 18 28 1 54 112 5 165 280 4 18 18 18 126 20 "));
    }
}

#endif