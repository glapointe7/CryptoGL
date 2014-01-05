/*
 * Source : http://paper.ijcsns.org/07_book/201105/20110502.pdf
 */
#ifndef HELLMANMERKLEKNAPSACKTEST_HPP
#define HELLMANMERKLEKNAPSACKTEST_HPP

#include <gtest/gtest.h>
#include "../src/HellmanMerkleKnapsack.hpp"
#include "../src/Digest.hpp"

class HellmanMerkleKnapsackTest : public ::testing::Test
{
protected:
   HellmanMerkleKnapsack *H;

   virtual void SetUp()
   {
      H = new HellmanMerkleKnapsack({24, 50, 91, 228, 434, 924, 1779, 3965}, 1843, 7908);
   }

   virtual void TearDown()
   {
      delete H;
   }
};

TEST_F(HellmanMerkleKnapsackTest, encode)
{
   EXPECT_EQ("6316 10022 10673 10673 15961", Digest::bigIntVectorToString(H->encode(String::toBytes("Hello"))));
}

TEST_F(HellmanMerkleKnapsackTest, decode)
{
   EXPECT_EQ("Hello", Vector::toString(H->decode(Digest::stringToBigIntVector("6316 10022 10673 10673 15961"))));
}

#endif