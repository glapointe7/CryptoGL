#ifndef BEAUFORTTEST_HPP
#define BEAUFORTTEST_HPP

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class BeaufortTest : public Test
    {
    protected:
       Beaufort *B;

       void setUp() override
       {
          B = new Beaufort("TESTING");
       }

       void tearDown() override
       {
          delete B;
       }
    };

    TEST(BeaufortEncode, BeaufortTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare("BEHZPAYIWHEIWGLLCZEUMRQFGIFOIEDCUHPTSGTPFSGQBLEANPAESZJNBKBIEMSZLPPQKSLYZB", B->encode(clear_text));

      /*B->setAlpha(String::alpha_numeric);
      B->setKey("V1Gen3rE");
      compare("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ", B->encode(clear_text));

      B->setAlpha(String::letters);
      B->setKey("vvvvvvvv");
      compare("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON", B->encode(clear_text));*/
    }

    TEST(BeaufortDecode, BeaufortTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      B->setKey("TESTING");
      compare(clear_text, B->decode("BEHZPAYIWHEIWGLLCZEUMRQFGIFOIEDCUHPTSGTPFSGQBLEANPAESZJNBKBIEMSZLPPQKSLYZB"));

      /*B->setAlpha(String::alpha_numeric);
      B->setKey("V1Gen3rE");
      compare(clear_text, B->decode("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ"));

      B->setAlpha(String::letters);
      B->setKey("vvvvvvvv");
      compare(clear_text, B->decode("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON"));*/
    }
}

#endif