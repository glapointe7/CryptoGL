#pragma once

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class VigenereTest : public Test
    {
    protected:
       Vigenere *V;
       static const StringTest clear_text;

       void setUp() override
       {
          V = new Vigenere("TESTING");
       }

       void tearDown() override
       {
          delete V;
       }
    };
    
    const StringTest VigenereTest::clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

    TEST(VigenereEncodeNormalCase, VigenereTest) 
    {
      compare("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL", V->encode(clear_text));
    }

    TEST(VigenereEncodeAlphaNum, VigenereTest) 
    {
      V->setAlpha(StringTest::alpha_numeric);
      V->setKey("V1Gen3rE");
      compare("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ", V->encode(clear_text));
    }

    TEST(VigenereEncodeLetters, VigenereTest) 
    {
      V->setAlpha(StringTest::letters);
      V->setKey("vvvvvvvv");
      compare("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON", V->encode(clear_text));
    }

    TEST(VigenereDecodeNormalCase, VigenereTest) 
    {
      V->setKey("TESTING");
      compare(clear_text, V->decode("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL"));
    }

    TEST(VigenereDecodeAlphaNum, VigenereTest) 
    {
      V->setAlpha(ClassicalType::alpha_numeric);
      V->setKey("V1Gen3rE");
      compare(clear_text, V->decode("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ"));
    }

    TEST(VigenereDecodeLetters, VigenereTest) 
    {
      V->setAlpha(ClassicalType::letters);
      V->setKey("vvvvvvvv");
      compare(clear_text, V->decode("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON"));
    }
}