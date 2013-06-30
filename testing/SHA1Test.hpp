#ifndef SHA1TEST_HPP
#define SHA1TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA.hpp"
#include "../src/Digest.hpp"

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
   std::string hash = "F5E6523299AC71B900615F641A4A74DB4C19A0AB";
   
   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

#endif