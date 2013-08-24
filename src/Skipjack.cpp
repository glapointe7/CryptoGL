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

const uint8_t Skipjack::F(const uint8_t half_block, const uint8_t round) const
{
   return f_table[half_block ^ subkeys[round]];
}

void Skipjack::encodeFeistelRounds(uint8_t& L, uint8_t& R, const uint8_t round) const
{
   L ^= F(R, round % 10);
   R ^= F(L, (round + 1) % 10);
   L ^= F(R, (round + 2) % 10);
   R ^= F(L, (round + 3) % 10);
}

void Skipjack::decodeFeistelRounds(uint8_t& L, uint8_t& R, const uint8_t round) const
{
   L ^= F(R, (round + 3) % 10);
   R ^= F(L, (round + 2) % 10);
   L ^= F(R, (round + 1) % 10);
   R ^= F(L, round % 10);
}

void Skipjack::applyRuleA(UInt16Container &input, const uint8_t round) const
{
   const uint16_t tmp = input[3];
   input[3] = input[2];
   input[2] = input[1];
   uint8_t L = input[0] >> 8;
   uint8_t R = input[0] & 0xFF;
   encodeFeistelRounds(L, R, round << 2);
   input[1] = (L << 8) | R;
   input[0] = tmp ^ input[1] ^ (round + 1);
}

void Skipjack::applyRuleB(UInt16Container &input, const uint8_t round) const
{
   const uint16_t tmp = input[3];
   input[3] = input[2];
   input[2] = input[0] ^ input[1] ^ (round + 1);
   uint8_t L = input[0] >> 8;
   uint8_t R = input[0] & 0xFF;
   encodeFeistelRounds(L, R, round << 2);
   input[1] = (L << 8) | R;
   input[0] = tmp;
}

void Skipjack::applyInverseRuleA(UInt16Container &input, const uint8_t round) const
{
   const uint16_t tmp = input[0] ^ input[1] ^ (round + 1);
   uint8_t R = input[1] >> 8;
   uint8_t L = input[1] & 0xFF;
   decodeFeistelRounds(L, R, round << 2);
   input[0] = (R << 8) | L;
   input[1] = input[2];
   input[2] = input[3];
   input[3] = tmp;
}

void Skipjack::applyInverseRuleB(UInt16Container &input, const uint8_t round) const
{
   const uint16_t tmp = input[0];
   uint8_t R = input[1] >> 8;
   uint8_t L = input[1] & 0xFF;
   decodeFeistelRounds(L, R, round << 2);
   input[0] = (R << 8) | L;
   input[1] = input[0] ^ input[2] ^ (round + 1);
   input[2] = input[3];
   input[3] = tmp;
}

const Skipjack::UInt16Container Skipjack::getIntegersFromInputBlock(const BytesContainer &block) const
{
   UInt16Container int_block;
   int_block.reserve(4);
   for (uint8_t i = 0; i < 8; i += 2)
   {
      int_block.push_back((block[i] << 8) | block[i + 1]);
   }
   
   return int_block;
}

const Skipjack::UInt16Container Skipjack::encodeBlock(const UInt16Container &input)
{
   UInt16Container encoded_block(input);
   for (uint8_t i = 0; i < 8; ++i)
      applyRuleA(encoded_block, i);
   for (uint8_t i = 8; i < 16; ++i)
      applyRuleB(encoded_block, i);
   for (uint8_t i = 16; i < 24; ++i)
      applyRuleA(encoded_block, i);
   for (uint8_t i = 24; i < 32; ++i)
      applyRuleB(encoded_block, i);
   
   return encoded_block;
}

const Skipjack::UInt16Container Skipjack::decodeBlock(const UInt16Container &input)
{
   UInt16Container decoded_block(input);
   for (uint8_t i = 31; i >= 24; --i)
      applyInverseRuleB(decoded_block, i);
   for (uint8_t i = 23; i >= 16; --i)
      applyInverseRuleA(decoded_block, i);
   for (uint8_t i = 15; i >= 8; --i)
      applyInverseRuleB(decoded_block, i);
   for (int8_t i = 7; i >= 0; --i)
      applyInverseRuleA(decoded_block, i);
   
   return decoded_block;
}

const Skipjack::BytesContainer Skipjack::getOutputBlock(const UInt16Container &int_block)
{
   BytesContainer output_block;
   output_block.reserve(8);
   for (uint8_t i = 0; i < 4; ++i)
   {
      output_block.push_back(int_block[i] >> 8);
      output_block.push_back(int_block[i] & 0xFF);
   }

   return output_block;
}