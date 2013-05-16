
#ifndef MD5TEST_HPP
#define	MD5TEST_HPP

#include <gtest/gtest.h>
#include "../src/MD5.hpp"

class MD5Test : public ::testing::Test
{
protected:
   MD5 *S;

   virtual void SetUp()
   {
      S = new MD5();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(MD5Test, encode)
{
   const std::string clear_text = "message digest";

   EXPECT_EQ("f96b697d7cb7938d525a2f31aaf161d0", MD5::hexDigest(S->encode(MD5::getBytesFromString(clear_text))));
}

#endif