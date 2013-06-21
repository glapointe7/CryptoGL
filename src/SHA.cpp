#include "SHA.hpp"

#include "Tools.hpp"

const HashFunction::WordsContainer SHA::round_constants = {
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void SHA::extendWords(WordsContainer &words, const uint8_t max_size)
{
   words.reserve(max_size - 16);
   for (uint8_t j = 16; j < max_size; ++j)
   {
      const uint32_t x = rotateRight(words[j-15], 7, 32) ^ rotateRight(words[j-15], 18, 32) ^ (words[j-15] >> 3);
      const uint32_t y = rotateRight(words[j-2], 19, 32) ^ rotateRight(words[j-2], 17, 32) ^ (words[j-2] >> 10);
      words.push_back(words[j-16] + x + words[j-7] + y);
   }
}

void SHA128::extendWords(WordsContainer &words, const uint8_t max_size)
{
   words.reserve(max_size - 16);
   for (uint8_t j = 16; j < max_size; ++j)
   {
      words.push_back(rotateLeft(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1, 32));
   }
}

const HashFunction::BytesContainer SHA128::encode(const BytesContainer &data)
{
   BytesContainer bits(appendPadding(data, Endianness::big_endian_64bits));
   const uint64_t bits_len = bits.size();

   /* States initialized in LITTLE ENDIAN. */
   WordsContainer state = {
      0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0
   };
   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      WordsContainer words = getBigEndianWordBlocks(bits, i);
      
      // Extention of the 32-bits 16 blocks in 80 blocks of 32 bits.
      extendWords(words, 80);
      
      WordsContainer hash(state);
      uint32_t f, k;
      for (uint8_t j = 0; j < 80; ++j)
      {
         if (j < 20)
         {
            f = F(hash[1], hash[2], hash[3]);
            k = 0x5A827999;
         }
         else if (j < 40)
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0x6ED9EBA1;
         }
         else if (j < 60)
         {
            f = G(hash[1], hash[2], hash[3]);
            k = 0x8F1BBCDC;
         }
         else
         {
            f = H(hash[1], hash[2], hash[3]);
            k = 0xCA62C1D6;
         }

         const uint32_t tmp = rotateLeft(hash[0], 5, 32) + f + hash[4] + k + words[j];
         hash[4] = hash[3];
         hash[3] = hash[2];
         hash[2] = rotateLeft(hash[1], 30, 32);
         hash[1] = hash[0];
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 5; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getBigEndianOutput(5, state);
}

const HashFunction::BytesContainer SHA224::encode(const BytesContainer &data)
{
   BytesContainer bits(appendPadding(data, Endianness::big_endian_64bits));
   const uint64_t bits_len = bits.size();

   WordsContainer state = {
      0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 
      0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
   };
   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      WordsContainer words = getBigEndianWordBlocks(bits, i);
      
      // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
      extendWords(words, 64);
      
      WordsContainer hash(state);
      for (uint8_t j = 0; j < 64; ++j)
      {
         const uint32_t S1 = rotateRight(hash[4], 6, 32) ^ rotateRight(hash[4], 11, 32) ^ rotateRight(hash[4], 25, 32);
         const uint32_t ch = (hash[4] & hash[5]) ^ (~hash[4] & hash[6]);
         const uint32_t tmp1 = hash[7] + S1 + ch + round_constants[j] + words[j];
         const uint32_t S0 = rotateRight(hash[0], 2, 32) ^ rotateRight(hash[0], 13, 32) ^ rotateRight(hash[0], 22, 32);
         const uint32_t maj = (hash[0] & hash[1]) ^ (hash[0] & hash[2]) ^ (hash[1] & hash[2]);
         const uint32_t tmp2 = S0 + maj;
        
         hash[7] = hash[6];
         hash[6] = hash[5];
         hash[5] = hash[4];
         hash[4] = hash[3] + tmp1;
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] = hash[0];
         hash[0] = tmp1 + tmp2;
      }
      
      for (uint8_t j = 0; j < 8; ++j)
      {
         state[j] += hash[j];
      }
   }
   
   return getBigEndianOutput(7, state);
}

const HashFunction::BytesContainer SHA256::encode(const BytesContainer &data)
{
   BytesContainer bits(appendPadding(data, Endianness::big_endian_64bits));
   const uint64_t bits_len = bits.size();

   WordsContainer state = {
      0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
      0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
   };
   for (uint64_t i = 0; i < bits_len; i += 64)
   {
      WordsContainer words = getBigEndianWordBlocks(bits, i);
      
      // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
      extendWords(words, 64);
      
      WordsContainer hash(state);
      for (uint8_t j = 0; j < 64; ++j)
      {
         const uint32_t S1 = rotateRight(hash[4], 6, 32) ^ rotateRight(hash[4], 11, 32) ^ rotateRight(hash[4], 25, 32);
         const uint32_t ch = (hash[4] & hash[5]) ^ (~hash[4] & hash[6]);
         const uint32_t tmp1 = hash[7] + S1 + ch + round_constants[j] + words[j];
         const uint32_t S0 = rotateRight(hash[0], 2, 32) ^ rotateRight(hash[0], 13, 32) ^ rotateRight(hash[0], 22, 32);
         const uint32_t maj = (hash[0] & hash[1]) ^ (hash[0] & hash[2]) ^ (hash[1] & hash[2]);
         const uint32_t tmp2 = S0 + maj;
        
         hash[7] = hash[6];
         hash[6] = hash[5];
         hash[5] = hash[4];
         hash[4] = hash[3] + tmp1;
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] = hash[0];
         hash[0] = tmp1 + tmp2;
      }
      
      for (uint8_t j = 0; j < 8; ++j)
      {
         state[j] += hash[j];
      }
   }
   
   return getBigEndianOutput(8, state);
}