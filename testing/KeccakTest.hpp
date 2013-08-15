
#ifndef KECCAKTEST_HPP
#define	KECCAKTEST_HPP

#include <gtest/gtest.h>
#include "../src/Keccak.hpp"
#include "../src/Digest.hpp"

class KeccakTest : public ::testing::Test
{
protected:
   Keccak224 *S;
   Keccak256 *T;
   Keccak384 *U;
   Keccak512 *V;

   virtual void SetUp()
   {
      S = new Keccak224();
      T = new Keccak256();
      U = new Keccak384();
      V = new Keccak512();
   }

   virtual void TearDown()
   {
      delete S;
      delete T;
      delete U;
      delete V;
   }
};

TEST_F(KeccakTest, encodeEmpty224)
{
   std::string hash = "F71837502BA8E10837BDD8D365ADB85591895602FC552B48B7390ABD";

   EXPECT_EQ(hash, Digest::hexDigest(S->encode(std::vector<uint8_t>(0))));
}

TEST_F(KeccakTest, encodeShort224)
{
   std::string hash = "6F9D2898EFD096BAAAAAB2E97482DDB6389B8E6CAA964B7A0E347E13";
   std::vector<uint8_t> cipher = {0x1F, 0x87, 0x7C};
   
   EXPECT_EQ(hash, Digest::hexDigest(S->encode(cipher)));
}

TEST_F(KeccakTest, encodeEmpty256)
{
   std::string hash = "C5D2460186F7233C927E7DB2DCC703C0E500B653CA82273B7BFAD8045D85A470";

   EXPECT_EQ(hash, Digest::hexDigest(T->encode(std::vector<uint8_t>(0))));
}

TEST_F(KeccakTest, encodeShort256)
{
   std::string hash = "627D7BC1491B2AB127282827B8DE2D276B13D7D70FB4C5957FDF20655BC7AC30";
   std::vector<uint8_t> cipher = {0x1F, 0x87, 0x7C};
   
   EXPECT_EQ(hash, Digest::hexDigest(T->encode(cipher)));
}

TEST_F(KeccakTest, encodeEmpty384)
{
   std::string hash = "2C23146A63A29ACF99E73B88F8C24EAA7DC60AA771780CCC006AFBFA8FE2479B2DD2B21362337441AC12B515911957FF";

   EXPECT_EQ(hash, Digest::hexDigest(U->encode(std::vector<uint8_t>(0))));
}

TEST_F(KeccakTest, encodeShort384)
{
   std::string hash = "B0665C345F45E6DE145B0190335EF5D5AA59E0B49FC1425D5EAE7355EA442284CB8A2152D565EBDF2810ECCAB15AF04F";
   std::vector<uint8_t> cipher = {0x1F, 0x87, 0x7C};
   
   EXPECT_EQ(hash, Digest::hexDigest(U->encode(cipher)));
}

TEST_F(KeccakTest, encodeLong512)
{
   std::string hash = "7690F703E894EE22D4DFF55A7F8D5021D5F17B729F95A59C4D55CFB225C67BE105F2E7CDF"
           "56D140E566648E9E9C39BBED96F985A6DAE1F21D8BA500F7FD40EDF";
   
   std::string digest = "83AF34279CCB5430FEBEC07A81950D30F4B66F484826AFEE7456F0071A51E1BBC55570B5"
           "CC7EC6F9309C17BF5BEFDD7C6BA6E968CF218A2B34BD5CF927AB846E38A40BBD81759E9E33381016A755F6"
           "99DF35D660007B5EADF292FEEFB735207EBF70B5BD17834F7BFA0E16CB219AD4AF524AB1EA37334AA66435"
           "E5D397FC0A065C411EBBCE32C240B90476D307CE802EC82C1C49BC1BEC48C0675EC2A6C6F3ED3E5B741D13"
           "437095707C565E10D8A20B8C20468FF9514FCF31B4249CD82DCEE58C0A2AF538B291A87E3390D737191A07"
           "484A5D3F3FB8C8F15CE056E5E5F8FEBE5E1FB59D6740980AA06CA8A0C20F5712B4CDE5D032E92AB89F0AE1";

   EXPECT_EQ(hash, Digest::hexDigest(V->encode(Digest::getBytesFromHexDigest(digest))));
}
     
TEST_F(KeccakTest, encodeEmpty512)
{
   std::string hash = "0EAB42DE4C3CEB9235FC91ACFFE746B29C29A8C366B7C60E4E67C466F36A4304C"
           "00FA9CAF9D87976BA469BCBE06713B435F091EF2769FB160CDAB33D3670680E";

   EXPECT_EQ(hash, Digest::hexDigest(V->encode(std::vector<uint8_t>(0))));
}

TEST_F(KeccakTest, encodeShort512)
{
   std::string hash = "EB7F2A98E00AF37D964F7D8C44C1FB6E114D8EE21A7B976AE736539EFDC1E3FE43BE"
           "CEF5015171E6DA30168CAE99A82C53FA99042774EF982C01626A540F08C0";

   std::vector<uint8_t> cipher = {0x1F, 0x87, 0x7C};
   
   EXPECT_EQ(hash, Digest::hexDigest(V->encode(cipher)));
}

#endif