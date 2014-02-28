#ifndef VIGENERETEST_HPP
#define VIGENERETEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"
#include "../src/String.hpp"

class VigenereTest : public ::testing::Test
{
protected:
   Vigenere *V;
   
   virtual void SetUp()
   {
      V = new Vigenere("TESTING");
   }

   virtual void TearDown()
   {
      delete V;
   }
};

TEST_F(VigenereTest, encode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL", V->encode(clear_text));
  
  V->setAlpha(ClassicalType::alpha_numeric);
  V->setKey("V1Gen3rE");
  EXPECT_EQ("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ", V->encode(clear_text));
  
  V->setAlpha(ClassicalType::letters);
  V->setKey("vvvvvvvv");
  EXPECT_EQ("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON", V->encode(clear_text));
}

TEST_F(VigenereTest, decode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  V->setKey("TESTING");
  EXPECT_EQ(clear_text, V->decode("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL"));
  
  V->setAlpha(ClassicalType::alpha_numeric);
  V->setKey("V1Gen3rE");
  EXPECT_EQ(clear_text, V->decode("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ"));
  
  V->setAlpha(ClassicalType::letters);
  V->setKey("vvvvvvvv");
  EXPECT_EQ(clear_text, V->decode("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON"));
}

#endif