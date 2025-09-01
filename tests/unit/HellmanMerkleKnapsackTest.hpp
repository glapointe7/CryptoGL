/*
 * Source : http://paper.ijcsns.org/07_book/201105/20110502.pdf
 */
#pragma once

#include "Test.hpp"
#include "../../src/algorithms/public_key/hellman_merkle/HellmanMerkleKnapsack.hpp"
#include "../../src/core/Utils/Digest.hpp"

namespace UnitTests
{
   class HellmanMerkleKnapsackTest : public Test
   {
   protected:
      HellmanMerkleKnapsack *H;
      const Vector<BigInteger> sequence = {
         BigInteger(24),
         BigInteger(50), 
         BigInteger(91), 
         BigInteger(228), 
         BigInteger(434), 
         BigInteger(924), 
         BigInteger(1779), 
         BigInteger(3965)
      };

      void setUp() override
      {
         H = new HellmanMerkleKnapsack(sequence, 1843, 7908);
      }

      void tearDown() override
      {
         delete H;
      }
   };

   TEST(HellmanMerkleKnapsackEncode, HellmanMerkleKnapsackTest)
   {
      compare("6316 10022 10673 10673 15961", BigInteger::toString(H->encode(StringTest("Hello").toBytes())));
   }

   TEST(HellmanMerkleKnapsackDecode, HellmanMerkleKnapsackTest)
   {
      compare("Hello", H->decode(BigInteger::stringToBigIntVector("0x18AC 0x2726 0x29B1 0x29B1 0x3E59")).toString());
   }
}