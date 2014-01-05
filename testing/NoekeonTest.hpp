/*
 * Source : https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/noekeon/Noekeon-Direct-128-128.verified.test-vectors
 */
#ifndef NOEKEONTEST_HPP
#define	NOEKEONTEST_HPP

#include <gtest/gtest.h>
#include "../src/Noekeon.hpp"
#include "../src/Digest.hpp"

class NoekeonTest : public ::testing::Test
{
protected:
   Noekeon *R, *C;

   virtual void SetUp()
   {
      R = new Noekeon({0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
      C = new Noekeon({0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00, 0x95, 0x2C, 0x49, 0x10, 0x48, 0x81, 0xFF, 0x48});
   }

   virtual void TearDown()
   {
      delete R;
      delete C;
   }
};

TEST_F(NoekeonTest, encodeZero)
{   
   EXPECT_EQ("EA6552BA793546C261E4B3E90433F5A2", Vector::toHexString(R->encode(std::vector<uint8_t>(16, 0))));
}

TEST_F(NoekeonTest, decodeZero)
{
   EXPECT_EQ("00000000000000000000000000000000", Vector::toHexString(R->decode(String::hexToBytes("EA6552BA793546C261E4B3E90433F5A2"))));
}

TEST_F(NoekeonTest, encodeNormal)
{
   const std::vector<uint8_t> clear_text = {
      0x47, 0x65, 0xF3, 0xDA, 0x10, 0xCD, 0x3D, 0x04, 0x73, 0x86, 0x77, 0x42, 0xB5, 0xE5, 0xCC, 0x3C
   };
   EXPECT_EQ("EA024714AD5C4D84EA024714AD5C4D84", Vector::toHexString(C->encode(clear_text)));
}

TEST_F(NoekeonTest, decodeNormal)
{
   EXPECT_EQ("4765F3DA10CD3D0473867742B5E5CC3C", Vector::toHexString(C->decode(String::hexToBytes("EA024714AD5C4D84EA024714AD5C4D84"))));
}

#endif