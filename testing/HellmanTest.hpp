#ifndef HELLMANTEST_HPP
#define HELLMANTEST_HPP

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
   const std::vector<uint64_t> expected = {29, 25, 22, 16, 22, 28, 22, 16,
   22, 16, 22, 44, 9, 16};
   
   H->setPrivateKey({1, 2, 4, 9});
   H->setDividend(15);
   H->setModulo(17);
   EXPECT_EQ(expected, H->encode(Hellman::stringToBytes(clear_text)));
}

TEST_F(HellmanTest, decode)
{
   const std::string clear_text = "Salaam!";
   const std::vector<uint64_t> expected = {29, 25, 22, 16, 22, 28, 22, 16,
   22, 16, 22, 44, 9, 16};
   
   H->setPrivateKey({1, 2, 4, 9});
   H->setDividend(15);
   H->setModulo(17);
   EXPECT_EQ(clear_text, Hellman::byteToString(H->decode(expected)));
}

#endif