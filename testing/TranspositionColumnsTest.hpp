#ifndef TRANSPOSITIONCOLUMNSTEST_HPP
#define TRANSPOSITIONCOLUMNSTEST_HPP

#include <gtest/gtest.h>
#include "../src/Transposition.hpp"

class TranspositionColumnsTest : public ::testing::Test
{
protected:
   TranspositionCompleteColumns *TC;

   virtual void SetUp()
   {
      TC = new TranspositionCompleteColumns({1u, 4u, 0u, 2u, 3u});
   }

   virtual void TearDown()
   {
      delete TC;
   }
};

TEST_F(TranspositionColumnsTest, encode)
{
   const std::string clear_text = "SALUTLESPETITSPOTS";

   EXPECT_EQ("LSTSSLTOUPSXTEPXAEIT", TC->encode(clear_text));
}

TEST_F(TranspositionColumnsTest, decode)
{
   const std::string clear_text = "SALUTLESPETITSPOTSXX";

   EXPECT_EQ(clear_text, TC->decode("LSTSSLTOUPSXTEPXAEIT"));
}

#endif