#include "Skipjack.hpp"

const uint8_t Skipjack::f_table[256] = {
   0xa3, 0xd7, 0x09, 0x83, 0xf8, 0x48, 0xf6, 0xf4, 0xb3, 0x21, 0x15, 0x78, 0x99, 0xb1, 0xaf, 0xf9,
   0xe7, 0x2d, 0x4d, 0x8a, 0xce, 0x4c, 0xca, 0x2e, 0x52, 0x95, 0xd9, 0x1e, 0x4e, 0x38, 0x44, 0x28,
   0x0a, 0xdf, 0x02, 0xa0, 0x17, 0xf1, 0x60, 0x68, 0x12, 0xb7, 0x7a, 0xc3, 0xe9, 0xfa, 0x3d, 0x53,
   0x96, 0x84, 0x6b, 0xba, 0xf2, 0x63, 0x9a, 0x19, 0x7c, 0xae, 0xe5, 0xf5, 0xf7, 0x16, 0x6a, 0xa2,
   0x39, 0xb6, 0x7b, 0x0f, 0xc1, 0x93, 0x81, 0x1b, 0xee, 0xb4, 0x1a, 0xea, 0xd0, 0x91, 0x2f, 0xb8,
   0x55, 0xb9, 0xda, 0x85, 0x3f, 0x41, 0xbf, 0xe0, 0x5a, 0x58, 0x80, 0x5f, 0x66, 0x0b, 0xd8, 0x90,
   0x35, 0xd5, 0xc0, 0xa7, 0x33, 0x06, 0x65, 0x69, 0x45, 0x00, 0x94, 0x56, 0x6d, 0x98, 0x9b, 0x76,
   0x97, 0xfc, 0xb2, 0xc2, 0xb0, 0xfe, 0xdb, 0x20, 0xe1, 0xeb, 0xd6, 0xe4, 0xdd, 0x47, 0x4a, 0x1d,
   0x42, 0xed, 0x9e, 0x6e, 0x49, 0x3c, 0xcd, 0x43, 0x27, 0xd2, 0x07, 0xd4, 0xde, 0xc7, 0x67, 0x18,
   0x89, 0xcb, 0x30, 0x1f, 0x8d, 0xc6, 0x8f, 0xaa, 0xc8, 0x74, 0xdc, 0xc9, 0x5d, 0x5c, 0x31, 0xa4,
   0x70, 0x88, 0x61, 0x2c, 0x9f, 0x0d, 0x2b, 0x87, 0x50, 0x82, 0x54, 0x64, 0x26, 0x7d, 0x03, 0x40,
   0x34, 0x4b, 0x1c, 0x73, 0xd1, 0xc4, 0xfd, 0x3b, 0xcc, 0xfb, 0x7f, 0xab, 0xe6, 0x3e, 0x5b, 0xa5,
   0xad, 0x04, 0x23, 0x9c, 0x14, 0x51, 0x22, 0xf0, 0x29, 0x79, 0x71, 0x7e, 0xff, 0x8c, 0x0e, 0xe2,
   0x0c, 0xef, 0xbc, 0x72, 0x75, 0x6f, 0x37, 0xa1, 0xec, 0xd3, 0x8e, 0x62, 0x8b, 0x86, 0x10, 0xe8,
   0x08, 0x77, 0x11, 0xbe, 0x92, 0x4f, 0x24, 0xc5, 0x32, 0x36, 0x9d, 0xcf, 0xf3, 0xa6, 0xbb, 0xac,
   0x5e, 0x6c, 0xa9, 0x13, 0x57, 0x25, 0xb5, 0xe3, 0xbd, 0xa8, 0x3a, 0x01, 0x05, 0x59, 0x2a, 0x46
};

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