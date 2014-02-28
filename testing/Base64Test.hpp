
#ifndef BASE64TEST_HPP
#define BASE64TEST_HPP

#include <gtest/gtest.h>
#include "../src/Base64.hpp"

class Base64Test : public ::testing::Test
{
protected:
   Base64 *A;

   virtual void SetUp()
   {
      A = new Base64();
   }

   virtual void TearDown()
   {
      delete A;
   }
};

TEST_F(Base64Test, encode) 
{
  const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM=", 
          A->encode(clear_text.toBytes()));
}

TEST_F(Base64Test, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ(clear_text, Vector::toString(A->decode(StringTest("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1"
          "JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM="))));
}

#endif