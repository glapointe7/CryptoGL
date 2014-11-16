#ifndef BEAUFORTGERMANTEST_HPP
#define BEAUFORTGERMANTEST_HPP

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class BeaufortGermanTest : public Test
    {
    protected:
       BeaufortGerman *V;

       void setUp() override
       {
          V = new BeaufortGerman("TESTING");
       }

       void tearDown() override
       {
          delete V;
       }
    };

    TEST(BeaufortGermanEncode, BeaufortGermanTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(StringTest("ZWTBLACSETWSEUPPYBWGOJKVUSVMSWXYGTLHIUHLVIUKZPWANLAWIBRNZQZSWOIBPLLKQIPCBZ"), V->encode(clear_text));

      /*V->setAlpha(String::alpha_numeric);
      V->setKey("V1Gen3rE");
      compare("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ", V->encode(clear_text));

      V->setAlpha(String::letters);
      V->setKey("vvvvvvvv");
      compare("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON", V->encode(clear_text));*/
    }

    TEST(BeaufortGermanDecode, BeaufortGermanTest) 
    {
      const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      V->setKey("TESTING");
      compare(clear_text, V->decode("ZWTBLACSETWSEUPPYBWGOJKVUSVMSWXYGTLHIUHLVIUKZPWANLAWIBRNZQZSWOIBPLLKQIPCBZ"));

      /*V->setAlpha(String::alpha_numeric);
      V->setKey("V1Gen3rE");
      compare(clear_text, V->decode("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ"));

      V->setAlpha(String::letters);
      V->setKey("vvvvvvvv");
      compare(clear_text, V->decode("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON"));*/
    }
}

#endif