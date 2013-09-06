#include "MessageDigest.hpp"

#include "Bits.hpp"

constexpr uint8_t MD4::left_rotation_table[];
constexpr uint8_t MD4::word_indexes[];
constexpr uint8_t MD5::left_rotation_table[];
constexpr uint32_t MD5::sine_magic_numbers[];

void MD4::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   uint32_t f, k = 0;
   for (uint8_t j = 0; j < rounds; ++j)
   {
      if (j < 16)
      {
         f = F(hash[1], hash[2], hash[3]);
      }
      else if (j < 32)
      {
         f = G(hash[1], hash[2], hash[3]);
         k = 0x5A827999;
      }
      else
      {
         f = H(hash[1], hash[2], hash[3]);
         k = 0x6ED9EBA1;
      }

      const uint32_t tmp = hash[3];
      hash[3] = hash[2];
      hash[2] = hash[1];
      hash[1] = Bits::rotateLeft(hash[0] + f + int_block[word_indexes[j]] + k, left_rotation_table[j], 32);
      hash[0] = tmp;
   }

   for (uint8_t j = 0; j < 4; ++j)
   {
      state[j] += hash[j];
   }
}

void MD5::compress(UInt32Vector &int_block, UInt32Vector &state)
{
   UInt32Vector hash(state);
   uint32_t f, k;
   for (uint8_t j = 0; j < rounds; ++j)
   {
      if (j < 16)
      {
         f = F(hash[1], hash[2], hash[3]);
         k = j;
      }
      else if (j < 32)
      {
         f = G(hash[1], hash[2], hash[3]);
         k = ((5 * j) + 1) & 0xF;
      }
      else if (j < 48)
      {
         f = H(hash[1], hash[2], hash[3]);
         k = ((3 * j) + 5) & 0xF;
      }
      else
      {
         f = I(hash[1], hash[2], hash[3]);
         k = (7 * j) & 0xF;
      }

      const uint32_t tmp = hash[3];
      hash[3] = hash[2];
      hash[2] = hash[1];
      hash[1] += Bits::rotateLeft(hash[0] + f + int_block[k] + sine_magic_numbers[j], left_rotation_table[j], 32);
      hash[0] = tmp;
   }

   for (uint8_t j = 0; j < 4; ++j)
   {
      state[j] += hash[j];
   }
}

const BytesVector MD4::encode(const BytesVector &data)
{
   BytesVector bytes = appendPadding(data);
   bytes = appendLength<LittleEndian64>(bytes, data.size() << 3);
   
   const uint64_t bytes_len = bytes.size();
   UInt32Vector state(IV);

   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += block_size)
   {
      UInt32Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, state);
   }

   return getOutput(state);
}

const BytesVector MD5::encode(const BytesVector &data)
{
   BytesVector bytes = appendPadding(data);
   bytes = appendLength<LittleEndian64>(bytes, data.size() << 3);
   
   const uint64_t bytes_len = bytes.size();
   UInt32Vector state(IV);

   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += block_size)
   {
      UInt32Vector int_block = getInputBlocks(bytes, i);
      compress(int_block, state);
   }

   return getOutput(state);
}