#include "Blowfish.hpp"

#include "exceptions/BadKeyLength.hpp"

// 18 initial sub-keys from the first decimals of PI.
const uint32_t Blowfish::P[18] = {
   0x243f6a88, 0x85a308d3, 0x13198a2e, 0x03707344, 0xa4093822, 0x299f31d0,
   0x082efa98, 0xec4e6c89, 0x452821e6, 0x38d01377, 0xbe5466cf, 0x34e90c6c,
   0xc0ac29b7, 0xc97c50dd, 0x3f84d5b5, 0xb5470917, 0x9216d5d9, 0x8979fb1b
};

void Blowfish::setKey(const BytesContainer &key)
{
   if (key.size() < 6 || key.size() > 56)
   {
      throw BadKeyLength("Your key length have to be between 6 bytes and 56 bytes.", key.size());
   }

   this->key = key;
}

// Process the Feistel algorithm modified for the blowfish algorithm.

void Blowfish::encodeRounds(uint64_t &L, uint64_t &R) const
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
   const uint64_t temp = L;
   L = R;
   R = temp;
}

void Blowfish::decodeRounds(uint64_t &L, uint64_t &R) const
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
   const uint64_t temp = L;
   L = R;
   R = temp;
}

void Blowfish::generateSubkeys()
{
   subkeys.reserve(18);
   const uint8_t key_len = key.size();

   for (uint8_t i = 0, j = 0; i < 18; ++i)
   {
      uint64_t data = 0;
      for (uint8_t k = 0; k < 4; ++k)
      {
         data = (data << 8) | key[j];
         j = (j + 1) % key_len;
      }
      subkeys.push_back(P[i] ^ data);
   }

   uint64_t L = 0;
   uint64_t R = 0;
   for (uint8_t i = 0; i < 18; i += 2)
   {
      encodeRounds(L, R);
      subkeys[i] = L;
      subkeys[i + 1] = R;
   }

   for (uint8_t i = 0; i < 4; ++i)
   {
      for (uint16_t j = 0; j < 256; j += 2)
      {
         encodeRounds(L, R);
         sbox[i][j] = L;
         sbox[i][j + 1] = R;
      }
   }
}

// Feistel function F.

uint64_t Blowfish::F(const uint64_t &data, const uint64_t &) const
{
   const uint8_t V[] = {
      static_cast<uint8_t>((data >> 24) & 0xFF),
      static_cast<uint8_t>((data >> 16) & 0xFF),
      static_cast<uint8_t>((data >> 8) & 0xFF),
      static_cast<uint8_t>(data & 0xFF)};

   return (((sbox[0][V[0]] + sbox[1][V[1]]) & 0xFFFFFFFF) ^ (sbox[2][V[2]])) + (sbox[3][V[3]] & 0xFFFFFFFF);
}

const Blowfish::BytesContainer
Blowfish::getOutputBlock(const BytesContainer &data, const bool to_encode)
{
   uint64_t value = 0;
   for (uint8_t j = 0, i = 56; j < 8; ++j, i -= 8)
   {
      const uint64_t x = data[j];
      value |= (x << i);
   }

   // Get the 18 sub-keys of 32 bits and process the 18 Feistel rounds.
   uint64_t L = (value >> 32) & 0xFFFFFFFF;
   uint64_t R = value & 0xFFFFFFFF;
   if (to_encode)
   {
      encodeRounds(L, R);
   }
   else
   {
      decodeRounds(L, R);
   }

   const uint64_t RL = (L << 32) | R;

   // Transform the encoded / decoded block to 8 blocks of 8 bits.
   BytesContainer output_block(8, 0);
   for (int8_t j = 7, i = 0; j >= 0; --j, i += 8)
   {
      output_block[j] = (RL >> i) & 0xFF;
   }

   return output_block;
}

const Blowfish::BytesContainer Blowfish::encode(const BytesContainer &clear_text)
{
   // Pad with 0x00 to get a multiple of 64 bits if needed.
   return process(addPadding(clear_text, 8, 0), true);
}

const Blowfish::BytesContainer Blowfish::decode(const BytesContainer &cipher_text)
{
   return process(cipher_text, false);
}