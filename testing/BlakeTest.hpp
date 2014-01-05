
#ifndef BLAKETEST_HPP
#define	BLAKETEST_HPP

#include <gtest/gtest.h>
#include "../src/Blake.hpp"
#include "../src/Digest.hpp"

class BlakeTest : public ::testing::Test
{
protected:
   Blake512 *S;
   Blake384 *T;
   Blake256 *U;
   Blake224 *V;

   virtual void SetUp()
   {
      S = new Blake512();
      T = new Blake384();
      U = new Blake256();
      V = new Blake224();
   }

   virtual void TearDown()
   {
      delete S;
      delete T;
      delete U;
      delete V;
   }
};

TEST_F(BlakeTest, encode512EmptyText)
{
   std::string hash = "A8CFBBD73726062DF0C6864DDA65DEFE58EF0CC52A5625090FA17601E1EECD1B62"
           "8E94F396AE402A00ACC9EAB77B4D4C2E852AAAA25A636D80AF3FC7913EF5B8";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(BlakeTest, encode384EmptyText)
{
   std::string hash = "C6CBD89C926AB525C242E6621F2F5FA73AA4AFE3D9E24AED727FAAADD6AF38B620BDB623DD2B4788B1C8086984AF8706";

   EXPECT_EQ(hash, Vector::toHexString(T->encode(std::vector<uint8_t>(0))));
}

TEST_F(BlakeTest, encode256EmptyText)
{
   std::string hash = "716F6E863F744B9AC22C97EC7B76EA5F5908BC5B2F67C61510BFC4751384EA7A";

   EXPECT_EQ(hash, Vector::toHexString(U->encode(std::vector<uint8_t>(0))));
}

TEST_F(BlakeTest, encode224EmptyText)
{
   std::string hash = "7DC5313B1C04512A174BD6503B89607AECBEE0903D40A8A569C94EED";

   EXPECT_EQ(hash, Vector::toHexString(V->encode(std::vector<uint8_t>(0))));
}

TEST_F(BlakeTest, encode512ShortText)
{
   const std::string clear_text = "119713CC83EEEF";
   std::string hash = "6E66BAE94DF2233958B1BE831AFA7678E247104CAFDF41C15AEADA5AC18715F1D4512114"
           "F299527A8434ED5DAA99B12BA7BB9465F6799CB0BFF9A31FD34C22D9";

   EXPECT_EQ(hash, Vector::toHexString(S->encode(String::hexToBytes(clear_text))));
}

TEST_F(BlakeTest, encode384ShortText)
{
   const std::string clear_text = "119713CC83EEEF";
   std::string hash = "3011CA63A5583CFE37EC4F3CF6477859D9C7BA11A72E5AD07F8A67D62DA643FA41617DB2EEDC017A8B43421B650B715C";

   EXPECT_EQ(hash, Vector::toHexString(T->encode(String::hexToBytes(clear_text))));
}

TEST_F(BlakeTest, encode256ShortText)
{
   const std::string clear_text = "119713CC83EEEF";
   std::string hash = "70C092FD5C8C21E9EF4BBC82A5C7819E262A530A748CAF285FF0CBA891954F1E";

   EXPECT_EQ(hash, Vector::toHexString(U->encode(String::hexToBytes(clear_text))));
}

TEST_F(BlakeTest, encode224ShortText)
{
   const std::string clear_text = "119713CC83EEEF";
   std::string hash = "F258E503C5CB61B124F5B8E154D639470A534ACA6B3B7FCCA7F683BC";

   EXPECT_EQ(hash, Vector::toHexString(V->encode(String::hexToBytes(clear_text))));
}

/*TEST_F(BlakeTest, encodeHMACText)
{
   const std::string clear_text = "Hi There";
   const std::vector<uint8_t> key = {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
      0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B};
   std::string hash = "87AA7CDEA5EF619D4FF0B4241A1D6CB02379F4E2CE4EC2787AD0B30545E1"
      "7CDEDAA833B7D6B8A702038B274EAEA3F4E4BE9D914EEB61F1702E696C203A126854";

   EXPECT_EQ(hash, Vector::toHexString(S->hmacEncode(key, String::toBytes(clear_text))));
}*/

#endif