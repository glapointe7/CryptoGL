#include "MD5.hpp"

#include "LittleEndian.hpp"
#include "Tools.hpp"
#include "ConverterTools.hpp"

const HashFunction::BytesContainer MD5::routine = {
   7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21,
   5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
   4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
   6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const HashFunction::WordsContainer MD5::K = {
   0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
   0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
   0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
   0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
   0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
   0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
   0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
   0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
   0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
   0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
   0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
   0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
   0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
   0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
   0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
   0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t MD5::F(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & y) | (~x & z);
}

uint32_t MD5::G(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return (x & z) | (y & ~z);
}

uint32_t MD5::H(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return x ^ y ^ z;
}

uint32_t MD5::I(const uint32_t x, const uint32_t y, const uint32_t z)
{
   return y ^ (x | ~z);
}

const HashFunction::BitsContainer MD5::addPadding(const BitsContainer &data_bits) const
{
   const uint64_t bits_len = data_bits.size();
   BitsContainer bits_pad(data_bits);
   bits_pad.reserve(bits_len + 576);

   // Append a bit '1' at the end.
   bits_pad.push_back(1);

   // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
   const uint32_t bits_pad_len = (960 - (bits_pad.size() & 0x1FF)) & 0x1FF;
   bits_pad.insert(bits_pad.end(), bits_pad_len, 0);

   // Pad with the 64-bits of the initial length of 'bits'.
   // Low-order 32-bit first.  
   
   for(int8_t i = 31; i >= 0; --i)
   {
      bits_pad.push_back((bits_len >> i) & 0x01);
   }
   for(uint8_t i = 63; i >= 32; --i)
   {
      bits_pad.push_back((bits_len >> i) & 0x01);
   }

   return bits_pad;
}

const HashFunction::WordsContainer MD5::getInput(const BitsContainer &bits, const uint32_t block_index) const
{
   WordsContainer words;
   words.reserve(16);
   LittleEndian4Bytes *LE = new LittleEndian4Bytes();
   
   for (uint16_t k = 0; k < 512; k += 32)
   {
      uint32_t word = 0;
      for(uint8_t i = 0; i < 32; ++i)
      {
         word |= bits[block_index + k + i] << (31 - i);
      }
      
      // Transform the word in little endian.
      LE->transform(word);
      words.push_back(LE->getValue());
      LE->reset();
   }
   
   return words;
}

const HashFunction::BytesContainer MD5::getOutput() const
{
   BytesContainer output;
   output.reserve(16);
   
   for (uint8_t j = 0; j < 4; ++j)
   {
      for(uint8_t i = 0; i < 4; ++i)
      {
         output.push_back((state[j] >> (i << 3)) & 0xFF);
      }
   }
   
   return output;
}

const HashFunction::BytesContainer MD5::encode(const BytesContainer &data)
{
   BitsContainer bits(addPadding(getBitsFromData(data)));
   const uint32_t bits_len = bits.size();

   // Assuming bits_len is a multiple of 512.
   for (uint32_t i = 0; i < bits_len; i += 512)
   {
      WordsContainer words = getInput(bits, i);
      WordsContainer hash(state);
      uint32_t f, g;
      for (uint8_t j = 0; j < 64; ++j)
      {
         if (j < 16)
         {
            f = F(hash[1], hash[2], hash[3]);
            g = j;
         }
         else if (j < 32)
         {
            f = G(hash[1], hash[2], hash[3]);
            g = ((5 * j) + 1) & 0xF;
         }
         else if (j < 48)
         {
            f = H(hash[1], hash[2], hash[3]);
            g = ((3 * j) + 5) & 0xF;
         }
         else
         {
            f = I(hash[1], hash[2], hash[3]);
            g = (7 * j) & 0xF;
         }

         const uint32_t tmp = hash[3];
         hash[3] = hash[2];
         hash[2] = hash[1];
         hash[1] += rotateLeft(hash[0] + f + words[g] + K[j], routine[j], 32);
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         state[j] += hash[j];
      }
   }
 
   return getOutput();
}