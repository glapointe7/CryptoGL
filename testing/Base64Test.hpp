
#ifndef BASE64TEST_HPP
#define	BASE64TEST_HPP

#include <gtest/gtest.h>
#include "../src/Base64.hpp"
#include "../src/Digest.hpp"

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
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM=", 
          A->encode(Digest::getBytesFromString(clear_text)));
}

TEST_F(Base64Test, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ(clear_text, Digest::getStringFromBytes(A->decode("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1"
          "JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM=")));
}

#endif