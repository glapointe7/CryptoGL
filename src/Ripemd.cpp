#include "Ripemd.hpp"

#include "Bits.hpp"

#include <algorithm>

constexpr uint32_t Ripemd::magic_numbers1[];
constexpr uint32_t Ripemd::magic_numbers_big2[];
constexpr uint32_t Ripemd::magic_numbers2[];
constexpr uint8_t Ripemd::word_selection1[];
constexpr uint8_t Ripemd::word_selection2[];
constexpr uint8_t Ripemd::left_shift1[];
constexpr uint8_t Ripemd::left_shift2[];

void Ripemd::process128_256(const UInt32Vector &words, UInt32Vector &hash, const uint8_t j)
{
   uint8_t k = 0;
   uint32_t f1, f2;
   if (j < 16)
   {
      f1 = F(hash[1], hash[2], hash[3]);
      f2 = I(hash[5], hash[6], hash[7]);
   }
   else if (j < 32)
   {
      f1 = G(hash[1], hash[2], hash[3]);
      f2 = H(hash[5], hash[6], hash[7]);
      k = 1;
   }
   else if (j < 48)
   {
      f1 = H(hash[1], hash[2], hash[3]);
      f2 = G(hash[5], hash[6], hash[7]);
      k = 2;
   }
   else
   {
      f1 = I(hash[1], hash[2], hash[3]);
      f2 = F(hash[5], hash[6], hash[7]);
      k = 3;
   }

   uint32_t tmp = Bits::rotateLeft(hash[0] + f1 + words[word_selection1[j]] + magic_numbers1[k], left_shift1[j], 32);
   hash[0] = hash[3];
   hash[3] = hash[2];
   hash[2] = hash[1];
   hash[1] = tmp;

   tmp = Bits::rotateLeft(hash[4] + f2 + words[word_selection2[j]] + magic_numbers2[k], left_shift2[j], 32);
   hash[4] = hash[7];
   hash[7] = hash[6];
   hash[6] = hash[5];
   hash[5] = tmp;
}

void Ripemd::process160_320(const UInt32Vector &words, UInt32Vector &hash, const uint8_t j)
{
   uint8_t k = 0;
   uint32_t f1, f2;
   if (j < 16)
   {
      f1 = F(hash[1], hash[2], hash[3]);
      f2 = J(hash[6], hash[7], hash[8]);
   }
   else if (j < 32)
   {
      f1 = G(hash[1], hash[2], hash[3]);
      f2 = I(hash[6], hash[7], hash[8]);
      k = 1;
   }
   else if (j < 48)
   {
      f1 = H(hash[1], hash[2], hash[3]);
      f2 = H(hash[6], hash[7], hash[8]);
      k = 2;
   }
   else if (j < 64)
   {
      f1 = I(hash[1], hash[2], hash[3]);
      f2 = G(hash[6], hash[7], hash[8]);
      k = 3;
   }
   else
   {
      f1 = J(hash[1], hash[2], hash[3]);
      f2 = F(hash[6], hash[7], hash[8]);
      k = 4;
   }

   uint32_t tmp = Bits::rotateLeft(hash[0] + f1 + words[word_selection1[j]] + magic_numbers1[k], left_shift1[j], 32) + hash[4];
   hash[0] = hash[4];
   hash[4] = hash[3];
   hash[3] = Bits::rotateLeft(hash[2], 10, 32);
   hash[2] = hash[1];
   hash[1] = tmp;

   tmp = Bits::rotateLeft(hash[5] + f2 + words[word_selection2[j]] + magic_numbers_big2[k], left_shift2[j], 32) + hash[9];
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

   for (uint8_t j = 0; j < 8; ++j)
   {
      state[j] += hash[j];
   }
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

   for (uint8_t j = 0; j < 10; ++j)
   {
      state[j] += hash[j];
   }
}

const BytesVector Ripemd128::encode(const BytesVector &data)
{
   BytesVector bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Vector states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += block_size)
   {
      UInt32Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, states);
   }

   return getOutput(states);
}

const BytesVector Ripemd160::encode(const BytesVector & data)
{
   BytesVector bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Vector states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += block_size)
   {
      UInt32Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, states);
   }

   return getOutput(states);
}

const BytesVector Ripemd256::encode(const BytesVector & data)
{
   BytesVector bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Vector states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += block_size)
   {
      UInt32Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, states);
   }

   return getOutput(states);
}

const BytesVector Ripemd320::encode(const BytesVector & data)
{
   BytesVector bytes = appendPadding(data);
   appendLength<LittleEndian64>(bytes, data.size() << 3);

   UInt32Vector states(IV);
   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += block_size)
   {
      UInt32Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, states);
   }

   return getOutput(states);
}