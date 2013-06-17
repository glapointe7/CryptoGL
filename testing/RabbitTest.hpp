
#ifndef RABBITTEST_HPP
#define	RABBITTEST_HPP

#include <gtest/gtest.h>
#include "../src/Rabbit.hpp"

class RabbitTest : public ::testing::Test
{
protected:
   Rabbit *R;

   virtual void SetUp()
   {
      const std::vector<uint8_t> key = {
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
      };
      R = new Rabbit(key);
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RabbitTest, encode)
{
   const std::vector<uint8_t> cipher = {
      0x02, 0xF7, 0x4A, 0x1C, 0x26, 0x45, 0x6B, 0xF5,
      0xEC, 0xD6, 0xA5, 0x36, 0xF0, 0x54, 0x57, 0xB1,
      0xA7, 0x8A, 0xC6, 0x89, 0x47, 0x6C, 0x69, 0x7B,
      0x39, 0x0C, 0x9C, 0xC5, 0x15, 0xD8, 0xE8, 0x88,
      0x96, 0xD6, 0x73, 0x16, 0x88, 0xD1, 0x68, 0xDA,
      0x51, 0xD4, 0x0C, 0x70, 0xC3, 0xA1, 0x16, 0xF4
   };

   const std::vector<uint8_t> clear_data(48, 0);

   EXPECT_EQ(Rabbit::hexDigest(cipher), Rabbit::hexDigest(R->encode(clear_data)));
}

TEST_F(RabbitTest, decode)
{
   const std::vector<uint8_t> cipher = {
      0x02, 0xF7, 0x4A, 0x1C, 0x26, 0x45, 0x6B, 0xF5,
      0xEC, 0xD6, 0xA5, 0x36, 0xF0, 0x54, 0x57, 0xB1,
      0xA7, 0x8A, 0xC6, 0x89, 0x47, 0x6C, 0x69, 0x7B,
      0x39, 0x0C, 0x9C, 0xC5, 0x15, 0xD8, 0xE8, 0x88,
      0x96, 0xD6, 0x73, 0x16, 0x88, 0xD1, 0x68, 0xDA,
      0x51, 0xD4, 0x0C, 0x70, 0xC3, 0xA1, 0x16, 0xF4
   };

   const std::vector<uint8_t> clear_data(48, 0);

   EXPECT_EQ(Rabbit::hexDigest(clear_data), Rabbit::hexDigest(R->decode(cipher)));
}

#endif