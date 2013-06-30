
#ifndef SHA512_256TEST_HPP
#define	SHA512_256TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA.hpp"
#include "../src/Digest.hpp"

class SHA512_256Test : public ::testing::Test
{
protected:
   SHA512_256 *S;

   virtual void SetUp()
   {
      S = new SHA512_256();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA512_256Test, encodeNormalText)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "DD9D67B371519C339ED8DBD25AF90E976A1EEEFD4AD3D889005E532FC5BEF04D";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(SHA512_256Test, encodeEmptyText)
{
   std::string hash = "C672B8D1EF56ED28AB87C3622C5114069BDD3AD7B8F9737498D0C01ECEF0967A";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif