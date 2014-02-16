
#ifndef GRONSFELDTEST_HPP
#define	GRONSFELDTEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"

class GronsfeldTest : public ::testing::Test
{
protected:
   Gronsfeld *V;
   
   virtual void SetUp()
   {
      V = new Gronsfeld({19, 4, 18, 19, 8, 13, 6});
   }

   virtual void TearDown()
   {
      delete V;
   }
};

TEST_F(GronsfeldTest, encode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL", V->encode(clear_text));
}

TEST_F(GronsfeldTest, decode) 
{
  const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ(clear_text, V->decode("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL"));
}

#endif