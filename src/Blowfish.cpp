#include "Blowfish.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "exceptions/EmptyKey.hpp"

// 18 initial sub-keys from the first decimals of PI.
const Blowfish::UInt64Container Blowfish::P = {
   0x243f6a88L, 0x85a308d3L, 0x13198a2eL, 0x03707344L, 0xa4093822L, 0x299f31d0L,
   0x082efa98L, 0xec4e6c89L, 0x452821e6L, 0x38d01377L, 0xbe5466cfL, 0x34e90c6cL,
   0xc0ac29b7L, 0xc97c50ddL, 0x3f84d5b5L, 0xb5470917L, 0x9216d5d9L, 0x8979fb1bL
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

void Blowfish::processFeistelRounds(uint64_t &L, uint64_t &R, const UInt64Container &subkeys,
        const uint8_t lower_round, const uint8_t rounds, const int8_t is_increasing)
{
   const uint8_t upper_round = lower_round + (rounds * is_increasing);
   uint8_t round;
   
   for (round = lower_round; round != upper_round; round += is_increasing << 1)
   {
      L ^= subkeys[round];
      R ^= F(L, subkeys[round]);
      R ^= subkeys[round + is_increasing];
      L ^= F(R, subkeys[round]);
   }
   
   L ^= subkeys[round];
   R ^= subkeys[round + is_increasing];
   const uint64_t temp = L;
   L = R;
   R = temp;
}

const Blowfish::UInt64Container Blowfish::getKeySchedule()
{
   UInt64Container subkeys;
   subkeys.reserve(18);
   const uint8_t key_len = key.size();

   uint8_t j = 0;
   for (uint8_t i = 0; i < 18; ++i)
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
      processFeistelRounds(L, R, subkeys, 0, 16, 1);
      subkeys[i] = L;
      subkeys[i + 1] = R;
   }

   for (uint8_t i = 0; i < 4; ++i)
   {
      for (uint16_t j = 0; j < 256; j += 2)
      {
         processFeistelRounds(L, R, subkeys, 0, 16, 1);
         sbox[i][j] = L;
         sbox[i][j + 1] = R;
      }
   }

   return subkeys;
}

// Feistel function F.

uint64_t Blowfish::F(const uint64_t &data, const uint64_t &) const
{
   BytesContainer V;
   V.reserve(4);

   V.push_back((data >> 24) & 0xFF);
   V.push_back((data >> 16) & 0xFF);
   V.push_back((data >> 8) & 0xFF);
   V.push_back(data & 0xFF);

   return (((sbox[0][V[0]] + sbox[1][V[1]]) & 0xFFFFFFFF) ^ (sbox[2][V[2]])) + (sbox[3][V[3]] & 0xFFFFFFFF);
}

const Blowfish::BytesContainer
Blowfish::getOutputBlock(const BytesContainer &data, const UInt64Container &subkeys, const uint8_t lower_round)
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
   if (lower_round - 15 < 0)
   {
      processFeistelRounds(L, R, subkeys, lower_round, 16, 1);
   }
   else
   {
      processFeistelRounds(L, R, subkeys, lower_round, 16, -1);
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
   return process(addPadding(clear_text, 8, 0), 0);
}

const Blowfish::BytesContainer Blowfish::decode(const BytesContainer &cipher_text)
{
   return process(cipher_text, 17);
}