
#ifndef HELLMANTEST_HPP
#define	HELLMANTEST_HPP

#include <gtest/gtest.h>
#include "../src/Hellman.hpp"

class HellmanTest : public ::testing::Test
{
protected:
   Hellman *H;
   
   virtual void SetUp()
   {
      H = new Hellman();
   }

   virtual void TearDown()
   {
      delete H;
   }
};

TEST_F(HellmanTest, encode)
{
   const std::string clear_text = "Salaam!";
   const std::vector<uint64_t> expected = {29ull, 25ull, 22ull, 16ull, 22ull, 28ull, 22ull, 16ull,
   22ull, 16ull, 22ull, 44ull, 9ull, 16ull};
   
   H->setPrivateKey({1ull, 2ull, 4ull, 9ull});
   H->setDividend(15ull);
   H->setModulo(17ull);
   EXPECT_EQ(expected, H->encode(Hellman::stringToBytes(clear_text)));
}

TEST_F(HellmanTest, decode)
{
   const std::string clear_text = "Salaam!";
   const std::vector<uint64_t> expected = {29ull, 25ull, 22ull, 16ull, 22ull, 28ull, 22ull, 16ull,
   22ull, 16ull, 22ull, 44ull, 9ull, 16ull};
   
   H->setPrivateKey({1ull, 2ull, 4ull, 9ull});
   H->setDividend(15ull);
   H->setModulo(17ull);
   EXPECT_EQ(clear_text, Hellman::byteToString(H->decode(expected)));
}

#endif