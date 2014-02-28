#ifndef TRANSPOSITIONDOUBLETEST_HPP
#define TRANSPOSITIONDOUBLETEST_HPP

#include <gtest/gtest.h>
#include "../src/Transposition.hpp"

class TranspositionDoubleTest : public ::testing::Test
{
protected:
   TranspositionDouble *TD;

   virtual void SetUp()
   {
      TD = new TranspositionDouble({2, 3, 7, 1, 6, 5, 0, 4}, {1, 6, 0, 7, 8, 4, 3, 5, 2});
   }

   virtual void TearDown()
   {
      delete TD;
   }
};

TEST_F(TranspositionDoubleTest, encode)
{
   const ClassicalType clear_text = "YOURMOTHERWASAHAMSTERANDYOURFATHERSMELTOFELDERBERRIES";

   EXPECT_EQ("NDODRWTRFHASEERAERMROFLBEOERSAYEAEIHMRALUTERHMTTYSOSU", TD->encode(clear_text));
}

TEST_F(TranspositionDoubleTest, decode)
{
   const ClassicalType clear_text = "YOURMOTHERWASAHAMSTERANDYOURFATHERSMELTOFELDERBERRIES";
 
   EXPECT_EQ(clear_text, TD->decode("NDODRWTRFHASEERAERMROFLBEOERSAYEAEIHMRALUTERHMTTYSOSU"));
}

#endif