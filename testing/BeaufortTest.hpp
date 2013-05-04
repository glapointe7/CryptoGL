
#ifndef BEAUFORTTEST_HPP
#define	BEAUFORTTEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"

class BeaufortTest : public ::testing::Test
{
protected:
   Beaufort *B;

   virtual void SetUp()
   {
      B = new Beaufort();
   }

   virtual void TearDown()
   {
      delete B;
   }
};

TEST_F(BeaufortTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  B->setKey("TESTING");
  EXPECT_EQ("BEHZPAYIWHEIWGLLCZEUMRQFGIFOIEDCUHPTSGTPFSGQBLEANPAESZJNBKBIEMSZLPPQKSLYZB", B->encode(clear_text));
  
  /*B->setAlpha(String::alpha_numeric);
  B->setKey("V1Gen3rE");
  EXPECT_EQ("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ", B->encode(clear_text));
  
  B->setAlpha(String::letters);
  B->setKey("vvvvvvvv");
  EXPECT_EQ("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON", B->encode(clear_text));*/
}

TEST_F(BeaufortTest, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  B->setKey("TESTING");
  EXPECT_EQ(clear_text, B->decode("BEHZPAYIWHEIWGLLCZEUMRQFGIFOIEDCUHPTSGTPFSGQBLEANPAESZJNBKBIEMSZLPPQKSLYZB"));
  
  /*B->setAlpha(String::alpha_numeric);
  B->setKey("V1Gen3rE");
  EXPECT_EQ(clear_text, B->decode("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ"));
  
  B->setAlpha(String::letters);
  B->setKey("vvvvvvvv");
  EXPECT_EQ(clear_text, B->decode("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON"));*/
}

#endif