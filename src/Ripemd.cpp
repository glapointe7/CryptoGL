#include "Ripemd.hpp"

#include "Tools.hpp"

#include <algorithm>

const uint32_t
Ripemd::magic_numbers1[5] = {0, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E};

const uint32_t
Ripemd::magic_numbers_big2[5] = {0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0};

const uint32_t
Ripemd::magic_numbers2[4] = {0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0};

const uint8_t Ripemd::word_selection1[80] = {
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
   7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
   3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
   1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
   4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13
};

const uint8_t Ripemd::word_selection2[80] = {
   5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
   6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
   15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
   8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
   12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11
};

const uint8_t Ripemd::left_shift1[80] = {
   11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
   7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
   11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
   11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
   9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6
};

const uint8_t Ripemd::left_shift2[80] = {
   8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
   9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
   9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
   15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
   8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11
};

void Ripemd::swapHashWithoutRotate(WordsContainer &hash, const uint32_t tmp)
{
   hash[0] = hash[3];
   hash[3] = hash[2];
   hash[2] = hash[1];
   hash[1] = tmp;
}

void Ripemd::swapHashWithRotate(WordsContainer &hash, const uint32_t tmp)
{
   hash[0] = hash[4];
   hash[4] = hash[3];
   hash[3] = rotateLeft(hash[2], 10, 32);
   hash[2] = hash[1];
   hash[1] = tmp;
}

void Ripemd::process128_256(const WordsContainer &words, WordsContainer &hash1, WordsContainer &hash2, const uint8_t j)
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

   uint32_t tmp = rotateLeft(hash1[0] + f1 + words[word_selection1[j]] + magic_numbers1[k], left_shift1[j], 32);
   swapHashWithoutRotate(hash1, tmp);

   tmp = rotateLeft(hash2[0] + f2 + words[word_selection2[j]] + magic_numbers2[k], left_shift2[j], 32);
   swapHashWithoutRotate(hash2, tmp);
}

void Ripemd::process160_320(const WordsContainer &words, WordsContainer &hash1, WordsContainer &hash2, const uint8_t j)
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

   uint32_t tmp = rotateLeft(hash1[0] + f1 + words[word_selection1[j]] + magic_numbers1[k], left_shift1[j], 32) + hash1[4];
   swapHashWithRotate(hash1, tmp);

   tmp = rotateLeft(hash2[0] + f2 + words[word_selection2[j]] + magic_numbers_big2[k], left_shift2[j], 32) + hash2[4];
   swapHashWithRotate(hash2, tmp);
}

const Ripemd::BytesContainer Ripemd128::encode(const BytesContainer & data)
{
   BytesContainer bytes(appendPadding(data));
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   WordsContainer states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const WordsContainer words = getInputBlocks(bytes, i, 16);
      WordsContainer hash1(states), hash2(states);

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
   BytesContainer bytes(appendPadding(data));
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   WordsContainer states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const WordsContainer words = getInputBlocks(bytes, i, 16);
      WordsContainer hash1(states), hash2(states);
      
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
   BytesContainer bytes(appendPadding(data));
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   WordsContainer states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const WordsContainer words = getInputBlocks(bytes, i, 16);
      WordsContainer hash1(states.begin(), states.begin() + 4), hash2(states.begin() + 4, states.end());

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
   BytesContainer bytes(appendPadding(data));
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   WordsContainer states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      const WordsContainer words = getInputBlocks(bytes, i, 16);
      WordsContainer hash1(states.begin(), states.begin() + 5), hash2(states.begin() + 5, states.end());

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