
#ifndef CHAOTEST_HPP
#define	CHAOTEST_HPP

#include <gtest/gtest.h>
#include "../src/Chao.hpp"

class ChaoTest : public ::testing::Test
{
protected:
   Chao *C;

   virtual void SetUp()
   {
      C = new Chao("HXUCZVAMDSLKPEFJRIGTWOBNYQ", "PTLNBQDEOYSFAVZKGJRIHWXUMC");
   }
   
   virtual void TearDown()
   {
      delete C;
   }
};

TEST_F(ChaoTest, encode)
{
   const ClassicalType clear_text = "WELLDONEISBETTERTHANWELLSAID";

   EXPECT_EQ("OAHQHCNYNXTSZJRRHJBYHQKSOUJY", C->encode(clear_text));
}

TEST_F(ChaoTest, decode)
{
   const ClassicalType clear_text = "WELLDONEISBETTERTHANWELLSAID";

   EXPECT_EQ(clear_text, C->decode("OAHQHCNYNXTSZJRRHJBYHQKSOUJY"));
}

#endif