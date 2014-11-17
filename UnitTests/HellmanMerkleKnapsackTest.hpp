/*
 * Source : http://paper.ijcsns.org/07_book/201105/20110502.pdf
 */
#ifndef HELLMANMERKLEKNAPSACKTEST_HPP
#define HELLMANMERKLEKNAPSACKTEST_HPP

#include "Test.hpp"
#include "../src/HellmanMerkleKnapsack.hpp"
#include "../src/Digest.hpp"

namespace UnitTests
{
    class HellmanMerkleKnapsackTest : public Test
    {
    protected:
       HellmanMerkleKnapsack *H;

       void setUp() override
       {
          H = new HellmanMerkleKnapsack({24, 50, 91, 228, 434, 924, 1779, 3965}, 1843, 7908);
       }

       void tearDown() override
       {
          delete H;
       }
    };

    TEST(HellmanMerkleKnapsackEncode, HellmanMerkleKnapsackTest)
    {
       compare(StringTest("6316 10022 10673 10673 15961"), Digest::bigIntVectorToString(H->encode(StringTest("Hello").toBytes())));
    }

    TEST(HellmanMerkleKnapsackDecode, HellmanMerkleKnapsackTest)
    {
       compare(StringTest("Hello"), H->decode(Digest::stringToBigIntVector("6316 10022 10673 10673 15961")).toString());
    }
}

#endif