#ifndef TRANSPOSITIONCOLUMNSTEST_HPP
#define TRANSPOSITIONCOLUMNSTEST_HPP

#include <gtest/gtest.h>
#include "../src/Transposition.hpp"

class TranspositionColumnsTest : public ::testing::Test
{
protected:
   TranspositionColumns *TC;

   virtual void SetUp()
   {
      TC = new TranspositionColumns();
   }

   virtual void TearDown()
   {
      delete TC;
   }
};

TEST_F(TranspositionColumnsTest, encode)
{
   const std::string clear_text = "SALUTLESPETITSPOTS";

   TC->setKey("GRAIN");
   EXPECT_EQ("LSTSSLTOUPSXTEPXAEIT", TC->encode(clear_text));
}

TEST_F(TranspositionColumnsTest, decode)
{
   const std::string clear_text = "SALUTLESPETITSPOTSXX";

   TC->setKey("GRAIN");
   EXPECT_EQ(clear_text, TC->decode("LSTSSLTOUPSXTEPXAEIT"));
}

#endif