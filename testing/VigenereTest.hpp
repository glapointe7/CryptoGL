
#ifndef VIGENERETEST_HPP
#define	VIGENERETEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"

class VigenereTest : public ::testing::Test
{
protected:
   Vigenere *V;

   virtual void SetUp()
   {
      V = new Vigenere();
   }

   virtual void TearDown()
   {
      delete V;
   }
};

TEST_F(VigenereTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  V->setKey("TESTING");
  EXPECT_EQ("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL", V->encode(clear_text));
  
  V->setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  V->setKey("V1Gen3rE");
  EXPECT_EQ("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ", V->encode(clear_text));
  
  V->setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
  V->setKey("vvvvvvvv");
  EXPECT_EQ("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON", V->encode(clear_text));
}

#endif