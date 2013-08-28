#include "HellmanMerkleKnapsack.hpp"

#include "MathematicalTools.hpp"
#include "Bits.hpp"

#include <algorithm>

HellmanMerkleKnapsack::HellmanMerkleKnapsack(const BigIntVector &sequence, const BigInteger &div, const BigInteger &mod)
{
   if(!Maths::areCoprimes(div, mod))
   {
      throw IntegersNotCoprimes("The modulo 'mod' and dividend 'r' have to be coprimes.");
   }
   
   if(div >= mod)
   {
      throw DividendGreaterThanModulo("Your modulo 'mod' has to be greater than your dividend 'r'.");
   }
   
   if(sequence.size() != 8)
   {
      throw BadSequenceLength("Your sequence's vector has to be of length 8.", sequence.size());
   }
   
   const BigInteger sum_sequence = isSuperIncresing(sequence);
   if(sum_sequence == 0)
   {
      throw SequenceNotSuperIncreasing("Your sequence's vector has to be a super increasing sequence.");
   }
   
   if(mod <= sum_sequence)
   {
      throw ModuloNotGreaterThanSequenceSum("Your modulo 'mod' has to be greater than the sum of the values in 'sequence'.");
   }
   
   this->div = div;
   this->mod = mod;
   this->sequence = sequence;
   
   makePublicKey();
}

const BigInteger HellmanMerkleKnapsack::isSuperIncresing(const BigIntVector &sequence)
{
   BigInteger sum;
   for (const auto number : sequence)
   {
      if (number <= sum)
      {
         return 0;
      }
      sum += number;
   }

   return sum;
}

void HellmanMerkleKnapsack::makePublicKey()
{
   public_key.reserve(sequence.size());
   for(const auto value : sequence)
   {
      public_key.push_back((value * div) % mod);
   }
}

uint8_t HellmanMerkleKnapsack::makePlainByte(const BigInteger &value)
{
   uint8_t byte = 0;
   BigInteger remaining = value;
   uint8_t pos = 7;
   while(remaining > 0)
   {
      while(remaining < sequence[pos])
      {
         pos--;
      }
      byte = Bits::setBitAtPosition(7-pos, byte);
      remaining -= sequence[pos];
   }
   
   return byte;
}

const HellmanMerkleKnapsack::BigIntVector HellmanMerkleKnapsack::encode(const BytesContainer &message)
{
   const uint64_t message_len = message.size();
   BigIntVector crypted;
   crypted.reserve(message_len);
   
   for(const auto byte : message)
   {
      BigInteger sum;
      for(uint8_t i = 0; i < 8; ++i)
      {
         sum += static_cast<BigInteger>(Bits::getBitAtPosition(7-i, byte)) * public_key[i];
      }
      crypted.push_back(sum);
   }
   
   return crypted;
}

const HellmanMerkleKnapsack::BytesContainer HellmanMerkleKnapsack::decode(const BigIntVector &cipher)
{
   BytesContainer decrypted;
   decrypted.reserve(cipher.size());
   
   const BigInteger inverse_div = Maths::getModInverse(div, mod);
   
   for(const auto value : cipher)
   {
      const BigInteger number = (value * inverse_div) % mod;
      decrypted.push_back(makePlainByte(number));
   }
   
   return decrypted;
}