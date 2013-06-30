#include "MessageDigest.hpp"

#include "Tools.hpp"

const uint8_t MD2::digits_of_pi[256] = {
   0x29, 0x2E, 0x43, 0xC9, 0xA2, 0xD8, 0x7C, 0x01, 0x3D, 0x36, 0x54, 0xA1, 0xEC, 0xF0, 0x06, 0x13,
   0x62, 0xA7, 0x05, 0xF3, 0xC0, 0xC7, 0x73, 0x8C, 0x98, 0x93, 0x2B, 0xD9, 0xBC, 0x4C, 0x82, 0xCA,
   0x1E, 0x9B, 0x57, 0x3C, 0xFD, 0xD4, 0xE0, 0x16, 0x67, 0x42, 0x6F, 0x18, 0x8A, 0x17, 0xE5, 0x12,
   0xBE, 0x4E, 0xC4, 0xD6, 0xDA, 0x9E, 0xDE, 0x49, 0xA0, 0xFB, 0xF5, 0x8E, 0xBB, 0x2F, 0xEE, 0x7A,
   0xA9, 0x68, 0x79, 0x91, 0x15, 0xB2, 0x07, 0x3F, 0x94, 0xC2, 0x10, 0x89, 0x0B, 0x22, 0x5F, 0x21,
   0x80, 0x7F, 0x5D, 0x9A, 0x5A, 0x90, 0x32, 0x27, 0x35, 0x3E, 0xCC, 0xE7, 0xBF, 0xF7, 0x97, 0x03,
   0xFF, 0x19, 0x30, 0xB3, 0x48, 0xA5, 0xB5, 0xD1, 0xD7, 0x5E, 0x92, 0x2A, 0xAC, 0x56, 0xAA, 0xC6,
   0x4F, 0xB8, 0x38, 0xD2, 0x96, 0xA4, 0x7D, 0xB6, 0x76, 0xFC, 0x6B, 0xE2, 0x9C, 0x74, 0x04, 0xF1,
   0x45, 0x9D, 0x70, 0x59, 0x64, 0x71, 0x87, 0x20, 0x86, 0x5B, 0xCF, 0x65, 0xE6, 0x2D, 0xA8, 0x02,
   0x1B, 0x60, 0x25, 0xAD, 0xAE, 0xB0, 0xB9, 0xF6, 0x1C, 0x46, 0x61, 0x69, 0x34, 0x40, 0x7E, 0x0F,
   0x55, 0x47, 0xA3, 0x23, 0xDD, 0x51, 0xAF, 0x3A, 0xC3, 0x5C, 0xF9, 0xCE, 0xBA, 0xC5, 0xEA, 0x26,
   0x2C, 0x53, 0x0D, 0x6E, 0x85, 0x28, 0x84, 0x09, 0xD3, 0xDF, 0xCD, 0xF4, 0x41, 0x81, 0x4D, 0x52,
   0x6A, 0xDC, 0x37, 0xC8, 0x6C, 0xC1, 0xAB, 0xFA, 0x24, 0xE1, 0x7B, 0x08, 0x0C, 0xBD, 0xB1, 0x4A,
   0x78, 0x88, 0x95, 0x8B, 0xE3, 0x63, 0xE8, 0x6D, 0xE9, 0xCB, 0xD5, 0xFE, 0x3B, 0x00, 0x1D, 0x39,
   0xF2, 0xEF, 0xB7, 0x0E, 0x66, 0x58, 0xD0, 0xE4, 0xA6, 0x77, 0x72, 0xF8, 0xEB, 0x75, 0x4B, 0x0A,
   0x31, 0x44, 0x50, 0xB4, 0x8F, 0xED, 0x1F, 0x1A, 0xDB, 0x99, 0x8D, 0x33, 0x9F, 0x11, 0x83, 0x14
};

const uint8_t MD4::left_rotation_table[48] = {
   3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19,
   3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13,
   3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15
};

const uint8_t MD4::word_indexes[48] = {
   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
   0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15,
   0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15
};

const uint8_t MD5::left_rotation_table[64] = {
   7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
   5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
   4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
   6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const uint32_t MD5::sine_magic_numbers[64] = {
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

const MessageDigest::BytesContainer MD2::appendPadding(const BytesContainer &data) const
{
   const uint64_t bytes_len = data.size();
   BytesContainer bytes_pad(data);

   // Pad with the value 'bytes_to_pad' at the end of bits_pad until the length is multiple of 16.
   uint8_t bytes_to_pad = 16 - (bytes_pad.size() & 0xF);
   bytes_pad.reserve(bytes_len + 16 + bytes_to_pad);
   bytes_pad.insert(bytes_pad.end(), bytes_to_pad, bytes_to_pad);

   return bytes_pad;
}

void MD2::process(const BytesContainer &data, BytesContainer &hash, const uint64_t &index)
{
   for (uint8_t j = 0; j < 16; ++j)
   {
      hash[16 + j] = data[index + j];
      hash[32 + j] = hash[j] ^ data[index + j];
   }

   // Process 18 rounds.
   uint8_t t = 0;
   for (uint8_t j = 0; j < 18; ++j)
   {
      for (uint8_t k = 0; k < 48; ++k)
      {
         t = hash[k] ^= digits_of_pi[t];
      }
      t = (t + j) & 0xFF;
   }
}

const MessageDigest::BytesContainer MD2::encode(const BytesContainer &data)
{
   const BytesContainer bytes(appendPadding(data));
   BytesContainer hash(48, 0);

   const uint64_t bytes_len = bytes.size();
   for (uint64_t i = 0; i < bytes_len; i += 16)
   {
      process(bytes, hash, i);

      // Update checksum.
      uint8_t t = checksum[15];
      for (uint8_t j = 0; j < 16; ++j)
      {
         t = checksum[j] ^= digits_of_pi[bytes[i + j] ^ t];
      }
   }

   // Last update of checksum.
   process(checksum, hash, 0);

   return BytesContainer(hash.begin(), hash.begin() + 16);
}

const MessageDigest::BytesContainer MD4::encode(const BytesContainer &data)
{
   BytesContainer bytes(appendPadding(data));
   const uint64_t bytes_len = bytes.size();

   /* Initial values. */
   WordsContainer state = {
      0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476
   };

   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      WordsContainer words = getInputBlocks(bytes, i);
      WordsContainer hash(state);
      uint32_t f, k = 0;
      for (uint8_t j = 0; j < 48; ++j)
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
         hash[1] = rotateLeft(hash[0] + f + words[word_indexes[j]] + k, left_rotation_table[j], 32);
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getOutput(4, state);
}

const MessageDigest::BytesContainer MD5::encode(const BytesContainer &data)
{
   BytesContainer bytes(appendPadding(data));
   const uint64_t bytes_len = bytes.size();

   /* Initial values. */
   WordsContainer state = {
      0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476
   };

   // Assuming bytes_len is a multiple of 64.
   for (uint64_t i = 0; i < bytes_len; i += 64)
   {
      WordsContainer words = getInputBlocks(bytes, i);
      WordsContainer hash(state);
      uint32_t f, k;
      for (uint8_t j = 0; j < 64; ++j)
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
         hash[1] += rotateLeft(hash[0] + f + words[k] + sine_magic_numbers[j], left_rotation_table[j], 32);
         hash[0] = tmp;
      }

      for (uint8_t j = 0; j < 4; ++j)
      {
         state[j] += hash[j];
      }
   }

   return getOutput(4, state);
}