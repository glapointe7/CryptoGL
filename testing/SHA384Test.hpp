
#ifndef SHA384TEST_HPP
#define	SHA384TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA.hpp"
#include "../src/Digest.hpp"

class SHA384Test : public ::testing::Test
{
protected:
   SHA384 *S;

   virtual void SetUp()
   {
      S = new SHA384();
   }

   virtual void TearDown()
   {
      delete S;
   }
};

TEST_F(SHA384Test, encodeNormalText)
{
   const std::string clear_text = "The quick brown fox jumps over the lazy dog";
   std::string hash = "CA737F1014A48F4C0B6DD43CB177B0AFD9E5169367544C494011E3317DBF9A509CB1E5DC1E85A941BBEE3D7F2AFBC9B1";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(Digest::getBytesFromString(clear_text))));
}

TEST_F(SHA384Test, encodeEmptyText)
{
   std::string hash = "38B060A751AC96384CD9327EB1B1E36A21FDB71114BE07434C0CC7BF63F6E1DA274EDEBFE76F65FBD51AD2F14898B95B";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(SHA384Test, encodeHMACText)
{
   const std::string clear_text = "Hi There";
   const std::vector<uint8_t> key = {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
      0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B};
   std::string hash = "AFD03944D84895626B0825F4AB46907F15F9DADBE4101EC682AA034C7CEB"
      "C59CFAEA9EA9076EDE7F4AF152E8B2FA9CB6";

   EXPECT_EQ(hash, Digest::hexDigest(S->hmacEncode(key, Digest::getBytesFromString(clear_text))));
}

#endif