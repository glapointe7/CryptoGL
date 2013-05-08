
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
      A = new Affine();
   }

   virtual void TearDown()
   {
      delete A;
   }
};

TEST_F(AffineTest, encode) 
{
  const std::string clear_text = "DIESISTEINEGEHEIMENACHRICHT";
  
  A->setKey(17, 7);
  EXPECT_EQ("GNXBNBSXNUXFXWXNDXUHPWKNPWS", A->encode(clear_text));
}

TEST_F(AffineTest, decode) 
{
  const std::string clear_text = "DIESISTEINEGEHEIMENACHRICHT";
  
  A->setKey(17, 7);
  EXPECT_EQ(clear_text, A->decode("GNXBNBSXNUXFXWXNDXUHPWKNPWS"));
}

#endif