#ifndef SHA1TEST_HPP
#define SHA1TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA1.hpp"
#include "../src/String.hpp"

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
   std::string hash = "f5e6523299ac71b900615f641a4a74db4c19a0ab";
   String::upperCase(hash);
   
   EXPECT_EQ(hash, SHA1::hexDigest(S->encode(SHA1::getBytesFromString(clear_text))));
}

#endif