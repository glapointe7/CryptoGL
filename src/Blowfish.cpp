#include "Blowfish.hpp"

#include "exceptions/BadKeyLength.hpp"

constexpr uint32_t Blowfish::P[];

void Blowfish::setKey(const BytesContainer &key)
{
   if (key.size() < 6 || key.size() > 56)
   {
      throw BadKeyLength("Your key length have to be between 6 bytes and 56 bytes.", key.size());
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
   const uint32_t temp = L;
   L = R;
   R = temp;
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
   const uint32_t temp = L;
   L = R;
   R = temp;
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
      static_cast<uint8_t>((half_block >> 24) & 0xFF),
      static_cast<uint8_t>((half_block >> 16) & 0xFF),
      static_cast<uint8_t>((half_block >> 8) & 0xFF),
      static_cast<uint8_t>(half_block & 0xFF)};

   return (((sbox[0][V[0]] + sbox[1][V[1]]) & 0xFFFFFFFF) ^ (sbox[2][V[2]])) + (sbox[3][V[3]] & 0xFFFFFFFF);
}

const uint64_t Blowfish::getIntegersFromInputBlock(const BytesContainer &block) const
{
   uint64_t value = 0;
   for (uint8_t j = 0, i = 56; j < 8; ++j, i -= 8)
   {
      const uint64_t x = block[j];
      value |= (x << i);
   }
   
   return value;
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
   BytesContainer output_block(8, 0);
   for (int8_t j = 7, i = 0; j >= 0; --j, i += 8)
   {
      output_block[j] = (int_block >> i) & 0xFF;
   }

   return output_block;
}