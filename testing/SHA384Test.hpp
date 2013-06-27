
#ifndef SHA384TEST_HPP
#define	SHA384TEST_HPP

#include <gtest/gtest.h>
#include "../src/SHA.hpp"

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

   EXPECT_EQ(hash, SHA384::hexDigest(S->encode(SHA384::getBytesFromString(clear_text))));
}

TEST_F(SHA384Test, encodeEmptyText)
{
   std::string hash = "38B060A751AC96384CD9327EB1B1E36A21FDB71114BE07434C0CC7BF63F6E1DA274EDEBFE76F65FBD51AD2F14898B95B";

   EXPECT_EQ(hash, SHA384::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

#endif