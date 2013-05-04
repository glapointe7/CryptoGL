
#ifndef SHA1TEST_HPP
#define	SHA1TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA1.hpp"

class SHA1Test : public ::testing::Test
{
protected:
   SHA1 *S;

   virtual void SetUp()
   {
      S = new SHA1();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA1Test, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   EXPECT_EQ("f5e6523299ac71b900615f641a4a74db4c19a0ab", SHA1::hexDigest(S->encode(SHA1::getBytesFromString(clear_text))));
}

#endif