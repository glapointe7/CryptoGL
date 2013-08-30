#include "Ripemd.hpp"

#include "Bits.hpp"

#include <algorithm>

constexpr uint32_t Ripemd::magic_numbers1[5];
constexpr uint32_t Ripemd::magic_numbers_big2[5];
constexpr uint32_t Ripemd::magic_numbers2[4];
constexpr uint8_t Ripemd::word_selection1[80];
constexpr uint8_t Ripemd::word_selection2[80];
constexpr uint8_t Ripemd::left_shift1[80];
constexpr uint8_t Ripemd::left_shift2[80];

void Ripemd::swapHashWithoutRotate(UInt32Container &hash, const uint32_t tmp)
{
   hash[0] = hash[3];
   hash[3] = hash[2];
   hash[2] = hash[1];
   hash[1] = tmp;
}

void Ripemd::swapHashWithRotate(UInt32Container &hash, const uint32_t tmp)
{
   hash[0] = hash[4];
   hash[4] = hash[3];
   hash[3] = Bits::rotateLeft(hash[2], 10, 32);
   hash[2] = hash[1];
   hash[1] = tmp;
}

void Ripemd::process128_256(const UInt32Container &words, UInt32Container &hash1, UInt32Container &hash2, const uint8_t j)
{
   uint8_t k = 0;
   uint32_t f1, f2;
   if (j < 16)
   {
      f1 = F(hash1[1], hash1[2], hash1[3]);
      f2 = I(hash2[1], hash2[2], hash2[3]);
   }
   else if (j < 32)
   {
      f1 = G(hash1[1], hash1[2], hash1[3]);
      f2 = H(hash2[1], hash2[2], hash2[3]);
      k = 1;
   }
   else if (j < 48)
   {
      f1 = H(hash1[1], hash1[2], hash1[3]);
      f2 = G(hash2[1], hash2[2], hash2[3]);
      k = 2;
   }
   else
   {
      f1 = I(hash1[1], hash1[2], hash1[3]);
      f2 = F(hash2[1], hash2[2], hash2[3]);
      k = 3;
   }

   uint32_t tmp = Bits::rotateLeft(hash1[0] + f1 + words[word_selection1[j]] + magic_numbers1[k], left_shift1[j], 32);
   swapHashWithoutRotate(hash1, tmp);

   tmp = Bits::rotateLeft(hash2[0] + f2 + words[word_selection2[j]] + magic_numbers2[k], left_shift2[j], 32);
   swapHashWithoutRotate(hash2, tmp);
}

void Ripemd::process160_320(const UInt32Container &words, UInt32Container &hash1, UInt32Container &hash2, const uint8_t j)
{
   uint8_t k = 0;
   uint32_t f1, f2;
   if (j < 16)
   {
      f1 = F(hash1[1], hash1[2], hash1[3]);
      f2 = J(hash2[1], hash2[2], hash2[3]);
   }
   else if (j < 32)
   {
      f1 = G(hash1[1], hash1[2], hash1[3]);
      f2 = I(hash2[1], hash2[2], hash2[3]);
      k = 1;
   }
   else if (j < 48)
   {
      f1 = H(hash1[1], hash1[2], hash1[3]);
      f2 = H(hash2[1], hash2[2], hash2[3]);
      k = 2;
   }
   else if (j < 64)
   {
      f1 = I(hash1[1], hash1[2], hash1[3]);
      f2 = G(hash2[1], hash2[2], hash2[3]);
      k = 3;
   }
   else
   {
      f1 = J(hash1[1], hash1[2], hash1[3]);
      f2 = F(hash2[1], hash2[2], hash2[3]);
      k = 4;
   }

   uint32_t tmp = Bits::rotateLeft(hash1[0] + f1 + words[word_selection1[j]] + magic_numbers1[k], left_shift1[j], 32) + hash1[4];
   swapHashWithRotate(hash1, tmp);

   tmp = Bits::rotateLeft(hash2[0] + f2 + words[word_selection2[j]] + magic_numbers_big2[k], left_shift2[j], 32) + hash2[4];
   swapHashWithRotate(hash2, tmp);
}

const Ripemd::BytesContainer Ripemd128::encode(const BytesContainer & data)
{
   BytesContainer bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Container states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const UInt32Container words = getInputBlocks(bytes, i);
      UInt32Container hash1(states), hash2(states);

      for (uint8_t j = 0; j < 64; ++j)
      {
         process128_256(words, hash1, hash2, j);
      }

      const uint32_t tmp = states[1] + hash1[2] + hash2[3];
      states[1] = states[2] + hash1[3] + hash2[0];
      states[2] = states[3] + hash1[0] + hash2[1];
      states[3] = states[0] + hash1[1] + hash2[2];
      states[0] = tmp;
   }

   return getOutput(4, states);
}

const Ripemd::BytesContainer Ripemd160::encode(const BytesContainer & data)
{
   BytesContainer bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Container states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const UInt32Container words = getInputBlocks(bytes, i);
      UInt32Container hash1(states), hash2(states);
      
      for (uint8_t j = 0; j < 80; ++j)
      {
         process160_320(words, hash1, hash2, j);
      }

      const uint32_t tmp = states[1] + hash1[2] + hash2[3];
      states[1] = states[2] + hash1[3] + hash2[4];
      states[2] = states[3] + hash1[4] + hash2[0];
      states[3] = states[4] + hash1[0] + hash2[1];
      states[4] = states[0] + hash1[1] + hash2[2];
      states[0] = tmp;
   }

   return getOutput(5, states);
}

const Ripemd::BytesContainer Ripemd256::encode(const BytesContainer & data)
{
   BytesContainer bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Container states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const UInt32Container words = getInputBlocks(bytes, i);
      UInt32Container hash1(states.begin(), states.begin() + 4), hash2(states.begin() + 4, states.end());

      for (uint8_t j = 0; j < 64; ++j)
      {
         process128_256(words, hash1, hash2, j);

         switch (j)
         {
            case 15: std::swap(hash1[0], hash2[0]);
               break;
            case 31: std::swap(hash1[1], hash2[1]);
               break;
            case 47: std::swap(hash1[2], hash2[2]);
               break;
            case 63: std::swap(hash1[3], hash2[3]);
               break;
         }
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         states[j] += hash1[j];
         states[j + 4] += hash2[j];
      }
   }

   return getOutput(8, states);
}

const Ripemd::BytesContainer Ripemd320::encode(const BytesContainer & data)
{
   BytesContainer bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Container states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const UInt32Container words = getInputBlocks(bytes, i);
      UInt32Container hash1(states.begin(), states.begin() + 5), hash2(states.begin() + 5, states.end());

      for (uint8_t j = 0; j < 80; ++j)
      {
         process160_320(words, hash1, hash2, j);

         switch (j)
         {
            case 15: std::swap(hash1[1], hash2[1]);
               break;
            case 31: std::swap(hash1[3], hash2[3]);
               break;
            case 47: std::swap(hash1[0], hash2[0]);
               break;
            case 63: std::swap(hash1[2], hash2[2]);
               break;
            case 79: std::swap(hash1[4], hash2[4]);
               break;
         }
      }

      for (uint8_t j = 0; j < 5; ++j)
      {
         states[j] += hash1[j];
         states[j + 5] += hash2[j];
      }
   }

   return getOutput(10, states);
}