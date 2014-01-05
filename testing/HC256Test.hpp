/*
 * Source : http://www.ecrypt.eu.org/stream/ciphers/hc256/hc256.pdf
 */
#ifndef HC256TEST_HPP
#define HC256TEST_HPP

#include <gtest/gtest.h>
#include "../src/HC256.hpp"
#include "../src/Digest.hpp"

class HC256Test : public ::testing::Test
{
protected:
   HC256 *R;

   virtual void SetUp()
   {
      R = new HC256(BytesVector(32, 0), BytesVector(32, 0));
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(HC256Test, encodeZero)
{
   const std::string out = "8589075B0DF3F6D82FC0C5425179B6A63465F053F2891F808B24744E18480B72"
      "EC2792CDBF4DCFEB7769BF8DFA14AEE47B4C50E8EAF3A9C8F506016C81697E32";
   
   EXPECT_EQ(out, Vector::toHexString(R->encode(BytesVector(64, 0))));
}

#endif