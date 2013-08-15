#include "Blowfish.hpp"

#include <algorithm>

#include "BigEndian.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr uint32_t Blowfish::P[];

void Blowfish::setKey(const BytesContainer &key)
{
   if (key.size() < 6 || key.size() > 56)
   {
      throw BadKeyLength("Your key length has to be between 6 bytes and 56 bytes.", key.size());
   }

   this->key = key;
}

// Process the Feistel algorithm modified for the blowfish algorithm.

void Blowfish::encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   uint8_t i;
   for (i = 0; i != rounds; i += 2)
   {
      L ^= subkeys[i];
      R ^= F(L, subkeys[i]);
      R ^= subkeys[i + 1];
      L ^= F(R, subkeys[i]);
   }
   
   L ^= subkeys[i];
   R ^= subkeys[i + 1];
   std::swap(L, R);
}

void Blowfish::decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const
{
   for (uint8_t i = rounds + 1; i != 1; i -= 2)
   {
      L ^= subkeys[i];
      R ^= F(L, subkeys[i]);
      R ^= subkeys[i - 1];
      L ^= F(R, subkeys[i]);
   }
   
   L ^= subkeys[1];
   R ^= subkeys[0];
   std::swap(L, R);
}

void Blowfish::generateSubkeys()
{
   subkeys.reserve(18);
   const uint8_t key_len = key.size();

   for (uint8_t i = 0, j = 0; i < 18; ++i)
   {
      uint32_t data = 0;
      for (uint8_t k = 0; k < 4; ++k)
      {
         data = (data << 8) | key[j];
         j = (j + 1) % key_len;
      }
      subkeys.push_back(P[i] ^ data);
   }

   uint32_t L = 0;
   uint32_t R = 0;
   for (uint8_t i = 0; i < 18; i += 2)
   {
      encodeFeistelRounds(L, R, i);
      subkeys[i] = L;
      subkeys[i + 1] = R;
   }

   for (uint8_t i = 0; i < 4; ++i)
   {
      for (uint16_t j = 0; j < 256; j += 2)
      {
         encodeFeistelRounds(L, R, j);
         sbox[i][j] = L;
         sbox[i][j + 1] = R;
      }
   }
}

// Feistel function F.

uint32_t Blowfish::F(const uint32_t half_block, const uint32_t) const
{
   const uint8_t V[] = {
      static_cast<uint8_t>(half_block >> 24),
      static_cast<uint8_t>(half_block >> 16),
      static_cast<uint8_t>(half_block >> 8),
      static_cast<uint8_t>(half_block)};

   return ((sbox[0][V[0]] + sbox[1][V[1]])  ^ (sbox[2][V[2]])) + sbox[3][V[3]];
}

const uint64_t Blowfish::getIntegersFromInputBlock(const BytesContainer &block) const
{
   BigEndian64 BE;
   BE.toInteger(block);
   
   return BE.getValue();
}

const uint64_t Blowfish::encodeBlock(const uint64_t &input)
{
   uint32_t L = input >> 32;
   uint32_t R = input & 0xFFFFFFFF;
   encodeFeistelRounds(L, R, 0);
   
   return (static_cast<uint64_t>(L) << 32) | R;
}

const uint64_t Blowfish::decodeBlock(const uint64_t &input)
{
   uint32_t L = input >> 32;
   uint32_t R = input & 0xFFFFFFFF;
   decodeFeistelRounds(L, R, 0);
   
   return (static_cast<uint64_t>(L) << 32) | R;
}

const Blowfish::BytesContainer Blowfish::getOutputBlock(const uint64_t &int_block)
{
   BigEndian64 BE;
   BE.toBytes(int_block);
   
   return BE.getBytes();
}