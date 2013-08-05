#include "Skipjack.hpp"

constexpr uint8_t Skipjack::f_table[256];

void Skipjack::setKey(const BytesContainer &key)
{
   const uint8_t key_len = key.size();
   if (key_len != 10)
   {
      throw BadKeyLength("Your key has to be 80 bits length.", key_len);
   }

   this->key = key;
}

void Skipjack::generateSubkeys()
{
   subkeys.reserve(10);
   subkeys.insert(subkeys.end(), key.begin(), key.end());
}

uint64_t Skipjack::F(const uint64_t &data, const uint64_t &subkey) const
{
   return f_table[data ^ subkey];
}

void Skipjack::encodeFeistelRounds(uint64_t& L, uint64_t& R, const uint8_t round) const
{
   L ^= F(R, subkeys[round % 10]);
   R ^= F(L, subkeys[(round + 1) % 10]);
   L ^= F(R, subkeys[(round + 2) % 10]);
   R ^= F(L, subkeys[(round + 3) % 10]);
}

void Skipjack::decodeFeistelRounds(uint64_t& L, uint64_t& R, const uint8_t round) const
{
   L ^= F(R, subkeys[(round + 3) % 10]);
   R ^= F(L, subkeys[(round + 2) % 10]);
   L ^= F(R, subkeys[(round + 1) % 10]);
   R ^= F(L, subkeys[round % 10]);
}

void Skipjack::applyRuleA(uint16_t *input, const uint8_t round) const
{
   const uint16_t tmp = input[3];
   input[3] = input[2];
   input[2] = input[1];
   uint64_t L = input[0] >> 8;
   uint64_t R = input[0] & 0xFF;
   encodeFeistelRounds(L, R, round << 2);
   input[1] = (L << 8) | R;
   input[0] = tmp ^ input[1] ^ (round + 1);
}

void Skipjack::applyRuleB(uint16_t *input, const uint8_t round) const
{
   const uint16_t tmp = input[3];
   input[3] = input[2];
   input[2] = input[0] ^ input[1] ^ (round + 1);
   uint64_t L = input[0] >> 8;
   uint64_t R = input[0] & 0xFF;
   encodeFeistelRounds(L, R, round << 2);
   input[1] = (L << 8) | R;
   input[0] = tmp;
}

void Skipjack::applyInverseRuleA(uint16_t *input, const uint8_t round) const
{
   const uint16_t tmp = input[0] ^ input[1] ^ (round + 1);
   uint64_t R = input[1] >> 8;
   uint64_t L = input[1] & 0xFF;
   decodeFeistelRounds(L, R, round << 2);
   input[0] = (R << 8) | L;
   input[1] = input[2];
   input[2] = input[3];
   input[3] = tmp;
}

void Skipjack::applyInverseRuleB(uint16_t *input, const uint8_t round) const
{
   const uint16_t tmp = input[0];
   uint64_t R = input[1] >> 8;
   uint64_t L = input[1] & 0xFF;
   decodeFeistelRounds(L, R, round << 2);
   input[0] = (R << 8) | L;
   input[1] = input[0] ^ input[2] ^ (round + 1);
   input[2] = input[3];
   input[3] = tmp;
}

const Skipjack::BytesContainer Skipjack::getOutputBlock(const BytesContainer &block, const bool to_encode)
{
   uint16_t input[4];
   for (uint8_t i = 0; i < 8; i += 2)
   {
      input[i >> 1] = (block[i] << 8) | block[i + 1];
   }

   if (to_encode)
   {
      for (uint8_t i = 0; i < 8; ++i)
         applyRuleA(input, i);
      for (uint8_t i = 8; i < 16; ++i)
         applyRuleB(input, i);
      for (uint8_t i = 16; i < 24; ++i)
         applyRuleA(input, i);
      for (uint8_t i = 24; i < 32; ++i)
         applyRuleB(input, i);
   }
   else
   {
      for (uint8_t i = 31; i >= 24; --i)
         applyInverseRuleB(input, i);
      for (uint8_t i = 23; i >= 16; --i)
         applyInverseRuleA(input, i);
      for (uint8_t i = 15; i >= 8; --i)
         applyInverseRuleB(input, i);
      for (int8_t i = 7; i >= 0; --i)
         applyInverseRuleA(input, i);
   }

   // Convert from 16-bit vector to 8-bit vector.
   BytesContainer output;
   output.reserve(8);
   for (uint8_t i = 0; i < 4; ++i)
   {
      output.push_back(input[i] >> 8);
      output.push_back(input[i] & 0xFF);
   }

   return output;
}

const Skipjack::BytesContainer Skipjack::encode(const BytesContainer &clear_text)
{
   return processEncoding(clear_text);
}

const Skipjack::BytesContainer Skipjack::decode(const BytesContainer &cipher_text)
{
   return processDecoding(cipher_text);
}