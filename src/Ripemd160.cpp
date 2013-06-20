#include "Ripemd160.hpp"

#include "Tools.hpp"

const HashFunction::WordsContainer
Ripemd160::magic_numbers1 = {0, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E};

const HashFunction::WordsContainer
Ripemd160::magic_numbers2 = {0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0};

const HashFunction::BytesContainer Ripemd160::word_selection1 = {
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
   7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
   3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
   1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
   4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13
};

const HashFunction::BytesContainer Ripemd160::word_selection2 = {
   5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
   6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
   15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
   8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
   12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11
};

const HashFunction::BytesContainer Ripemd160::left_shift1 = {
   11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
   7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
   11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
   11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
   9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6
};

const HashFunction::BytesContainer Ripemd160::left_shift2 = {
   8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
   9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
   9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
   15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
   8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11
};

uint32_t Ripemd160::F(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return x ^ y ^ z;
}

uint32_t Ripemd160::G(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & y) | (~x & z);
}

uint32_t Ripemd160::H(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x | ~y) ^ z;
}

uint32_t Ripemd160::I(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & z) | (y & ~z);
}

uint32_t Ripemd160::J(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return x ^ (y | ~z);
}

const HashFunction::BytesContainer Ripemd160::appendPadding(const BytesContainer &data) const
{
   const uint64_t bytes_len = data.size() << 3;
   BytesContainer bytes_pad(data);
   bytes_pad.reserve((bytes_len >> 3) + 64);

   // Append a bit '1' at the end.
   bytes_pad.push_back(0x80);

   // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
   const uint8_t bytes_pad_len = (120 - (bytes_pad.size() & 0x3F)) & 0x3F;
   bytes_pad.insert(bytes_pad.end(), bytes_pad_len, 0);

   // Append the 64-bit of the initial bits length of data in little-endian.    
   for (uint8_t i = 0; i < 64; i += 8)
   {
      bytes_pad.push_back((bytes_len >> i) & 0xFF);
   }

   return bytes_pad;
}

const HashFunction::WordsContainer 
Ripemd160::getWordBlocks(const BytesContainer &bytes, const uint64_t &block_index) const
{
   WordsContainer words;
   words.reserve(16);

   // 32-bit integers in little-endian : bytes[3]bytes[2]bytes[1]bytes[0].
   for (uint8_t k = 0; k < 64; k += 4)
   {
      const uint32_t word = bytes[k + block_index]
              | (bytes[k + block_index + 1] << 8)
              | (bytes[k + block_index + 2] << 16)
              | (bytes[k + block_index + 3] << 24);

      words.push_back(word);
   }

   return words;
}

const HashFunction::BytesContainer Ripemd160::getOutput() const
{
   BytesContainer output;
   output.reserve(20);

   for (uint8_t j = 0; j < 5; ++j)
   {
      for(uint8_t i = 0; i < 32; i += 8)
      {
         output.push_back((state[j] >> i) & 0xFF);
      }
   }

   return output;
}

const HashFunction::BytesContainer Ripemd160::encode(const BytesContainer &data)
{
   BytesContainer bytes(appendPadding(data));
   const uint64_t bytes_len = bytes.size();

   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      WordsContainer words = getWordBlocks(bytes, i);
      WordsContainer hash1(state), hash2(state);
      uint32_t f1, f2, k1, k2;
      for (uint8_t j = 0; j < 80; ++j)
      {
         if (j < 16)
         {
            f1 = F(hash1[1], hash1[2], hash1[3]);
            f2 = J(hash2[1], hash2[2], hash2[3]);
            k1 = magic_numbers1[0];
            k2 = magic_numbers2[0];
         }
         else if (j < 32)
         {
            f1 = G(hash1[1], hash1[2], hash1[3]);
            f2 = I(hash2[1], hash2[2], hash2[3]);
            k1 = magic_numbers1[1];
            k2 = magic_numbers2[1];
         }
         else if (j < 48)
         {
            f1 = H(hash1[1], hash1[2], hash1[3]);
            f2 = H(hash2[1], hash2[2], hash2[3]);
            k1 = magic_numbers1[2];
            k2 = magic_numbers2[2];
         }
         else if(j < 64)
         {
            f1 = I(hash1[1], hash1[2], hash1[3]);
            f2 = G(hash2[1], hash2[2], hash2[3]);
            k1 = magic_numbers1[3];
            k2 = magic_numbers2[3];
         }
         else
         {
            f1 = J(hash1[1], hash1[2], hash1[3]);
            f2 = F(hash2[1], hash2[2], hash2[3]);
            k1 = magic_numbers1[4];
            k2 = magic_numbers2[4];
         }
         
         uint32_t tmp = rotateLeft(hash1[0] + f1 + words[word_selection1[j]] + k1, left_shift1[j], 32) + hash1[4];
         hash1[0] = hash1[4];
         hash1[4] = hash1[3];
         hash1[3] = rotateLeft(hash1[2], 10, 32);
         hash1[2] = hash1[1];
         hash1[1] = tmp;
         
         tmp = rotateLeft(hash2[0] + f2 + words[word_selection2[j]] + k2, left_shift2[j], 32) + hash2[4];
         hash2[0] = hash2[4];
         hash2[4] = hash2[3];
         hash2[3] = rotateLeft(hash2[2], 10, 32);
         hash2[2] = hash2[1];
         hash2[1] = tmp;
      }

      const uint32_t tmp = state[1] + hash1[2] + hash2[3];
      state[1] = state[2] + hash1[3] + hash2[4];
      state[2] = state[3] + hash1[4] + hash2[0];
      state[3] = state[4] + hash1[0] + hash2[1];
      state[4] = state[0] + hash1[1] + hash2[2];
      state[0] = tmp;
   }

   return getOutput();
}