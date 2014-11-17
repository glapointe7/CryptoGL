#ifndef ROZIERTEST_HPP
#define ROZIERTEST_HPP

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class RozierTest : public Test
    {
    protected:
       Rozier *R;

       void setUp() override
       {
          R = new Rozier("TESTING");
       }

       void tearDown() override
       {
          delete R;
       }
    };

    TEST(RozierEncode, RozierTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(StringTest("DOMJYGVWWMEFKNTHRJJMHNCOCFBFWOQGTZELANPYBBYCSXJGGPSPQOXGDISAJUBFHETXWBTUUH"), R->encode(clear_text));

      /*R->setAlpha(String::alpha_numeric);
      R->setKey("V1Gen3rE");
      compare("yPjdjBfrXjYQFXoiodUHridlWQ6prPnae4ogbkJj6ltdpRUBqkTmKZ2qyjpUUzl0ibNi1loVrb", R->encode(clear_text));

      R->setAlpha(String::letters);
      R->setKey("vvvvvvvv");
      compare(clear_text, R->encode(clear_text));*/
    }

    TEST(RozierDecode, RozierTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      R->setKey("TESTING");
      compare(clear_text, R->decode("DOMJYGVWWMEFKNTHRJJMHNCOCFBFWOQGTZELANPYBBYCSXJGGPSPQOXGDISAJUBFHETXWBTUUH"));

      /*R->setAlpha(String::alpha_numeric);
      R->setKey("V1Gen3rE");
      compare(clear_text, R->decode("yPjdjBfrXjYQFXoiodUHridlWQ6prPnae4ogbkJj6ltdpRUBqkTmKZ2qyjpUUzl0ibNi1loVrb"));

      R->setAlpha(String::letters);
      R->setKey("vvvvvvvv");
      compare(clear_text, R->decode(clear_text));*/
    }
}

#endif