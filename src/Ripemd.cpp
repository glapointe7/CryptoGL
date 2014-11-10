#include "Ripemd.hpp"

#include "Bits.hpp"

#include <algorithm>

using namespace CryptoGL;

constexpr std::array<uint32_t, 5> Ripemd::magic_numbers1;
constexpr std::array<uint32_t, 5> Ripemd::magic_numbers_big2;
constexpr std::array<uint32_t, 4> Ripemd::magic_numbers2;
constexpr std::array<uint8_t, 80> Ripemd::word_selection1;
constexpr std::array<uint8_t, 80> Ripemd::word_selection2;
constexpr std::array<uint8_t, 80> Ripemd::left_shift1;
constexpr std::array<uint8_t, 80> Ripemd::left_shift2;

void Ripemd::process128_256(const UInt32Vector &words, UInt32Vector &hash, const uint8_t j)
{
   const uint8_t index = j >> 4;
   uint32_t f1, f2;
   switch(index)
   {
      case 0:
         f1 = F(hash[1], hash[2], hash[3]);
         f2 = I(hash[5], hash[6], hash[7]);
         break;
         
      case 1:
         f1 = G(hash[1], hash[2], hash[3]);
         f2 = H(hash[5], hash[6], hash[7]);
         break;
         
      case 2:
         f1 = H(hash[1], hash[2], hash[3]);
         f2 = G(hash[5], hash[6], hash[7]);
         break;
         
      case 3:
         f1 = I(hash[1], hash[2], hash[3]);
         f2 = F(hash[5], hash[6], hash[7]);
         break;
   }

   uint32_t tmp = Bits::rotateLeft(hash[0] + f1 + words[word_selection1[j]] + magic_numbers1[index], left_shift1[j], 32);
   hash[0] = hash[3];
   hash[3] = hash[2];
   hash[2] = hash[1];
   hash[1] = tmp;

   tmp = Bits::rotateLeft(hash[4] + f2 + words[word_selection2[j]] + magic_numbers2[index], left_shift2[j], 32);
   hash[4] = hash[7];
   hash[7] = hash[6];
   hash[6] = hash[5];
   hash[5] = tmp;
}

void Ripemd::process160_320(const UInt32Vector &words, UInt32Vector &hash, const uint8_t j)
{
   const uint8_t index = j >> 4;
   uint32_t f1, f2;
   switch(index)
   {
      case 0:
         f1 = F(hash[1], hash[2], hash[3]);
         f2 = J(hash[6], hash[7], hash[8]);
         break;
         
      case 1:
         f1 = G(hash[1], hash[2], hash[3]);
         f2 = I(hash[6], hash[7], hash[8]);
         break;
         
      case 2:
         f1 = H(hash[1], hash[2], hash[3]);
         f2 = H(hash[6], hash[7], hash[8]);
         break;
         
      case 3:
         f1 = I(hash[1], hash[2], hash[3]);
         f2 = G(hash[6], hash[7], hash[8]);
         break;
         
      case 4:
         f1 = J(hash[1], hash[2], hash[3]);
         f2 = F(hash[6], hash[7], hash[8]);
         break;
   }
   
   uint32_t tmp = Bits::rotateLeft(hash[0] + f1 + words[word_selection1[j]] + magic_numbers1[index], left_shift1[j], 32) + hash[4];
   hash[0] = hash[4];
   hash[4] = hash[3];
   hash[3] = Bits::rotateLeft(hash[2], 10, 32);
   hash[2] = hash[1];
   hash[1] = tmp;

   tmp = Bits::rotateLeft(hash[5] + f2 + words[word_selection2[j]] + magic_numbers_big2[index], left_shift2[j], 32) + hash[9];
   hash[5] = hash[9];
   hash[9] = hash[8];
   hash[8] = Bits::rotateLeft(hash[7], 10, 32);
   hash[7] = hash[6];
   hash[6] = tmp;
}

void Ripemd128::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   hash.reserve(8);
   hash.insert(hash.end(), state.begin(), state.end());
   for (uint8_t j = 0; j < rounds; ++j)
   {
      process128_256(int_block, hash, j);
   }

   const uint32_t tmp = state[1] + hash[2] + hash[7];
   state[1] = state[2] + hash[3] + hash[4];
   state[2] = state[3] + hash[0] + hash[5];
   state[3] = state[0] + hash[1] + hash[6];
   state[0] = tmp;
}

void Ripemd160::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   hash.reserve(10);
   hash.insert(hash.end(), state.begin(), state.end());
   for (uint8_t j = 0; j < rounds; ++j)
   {
      process160_320(int_block, hash, j);
   }

   const uint32_t tmp = state[1] + hash[2] + hash[8];
   state[1] = state[2] + hash[3] + hash[9];
   state[2] = state[3] + hash[4] + hash[5];
   state[3] = state[4] + hash[0] + hash[6];
   state[4] = state[0] + hash[1] + hash[7];
   state[0] = tmp;
}

void Ripemd256::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   for (uint8_t j = 0; j < rounds; ++j)
   {
      process128_256(int_block, hash, j);

      switch (j)
      {
         case 15: std::swap(hash[0], hash[4]);
            break;
         case 31: std::swap(hash[1], hash[5]);
            break;
         case 47: std::swap(hash[2], hash[6]);
            break;
         case 63: std::swap(hash[3], hash[7]);
            break;
      }
   }

   applyDaviesMayerFunction(hash, state);
}

void Ripemd320::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   for (uint8_t j = 0; j < rounds; ++j)
   {
      process160_320(int_block, hash, j);

      switch (j)
      {
         case 15: std::swap(hash[1], hash[6]);
            break;
         case 31: std::swap(hash[3], hash[8]);
            break;
         case 47: std::swap(hash[0], hash[5]);
            break;
         case 63: std::swap(hash[2], hash[7]);
            break;
         case 79: std::swap(hash[4], hash[9]);
            break;
      }
   }

   applyDaviesMayerFunction(hash, state);
}