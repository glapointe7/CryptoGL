
#ifndef AFFINETEST_HPP
#define	AFFINETEST_HPP

#include <gtest/gtest.h>
#include "../src/Affine.hpp"

class AffineTest : public ::testing::Test
{
protected:
   Affine *A;

   virtual void SetUp()
   {
      A = new Affine(17, 7);
   }

   virtual void TearDown()
   {
      delete A;
   }
};

TEST_F(AffineTest, encode) 
{
  const ClassicalType clear_text = "DIESISTEINEGEHEIMENACHRICHT";
  
  EXPECT_EQ("GNXBNBSXNUXFXWXNDXUHPWKNPWS", A->encode(clear_text));
}

TEST_F(AffineTest, decode) 
{
  const ClassicalType clear_text = "DIESISTEINEGEHEIMENACHRICHT";
  
  EXPECT_EQ(clear_text, A->decode("GNXBNBSXNUXFXWXNDXUHPWKNPWS"));
}

#endif