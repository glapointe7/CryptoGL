
#ifndef MESSAGEDIGEST_HPP
#define	MESSAGEDIGEST_HPP

#include "HashFunction.hpp"
#include "LittleEndian.hpp"

class MessageDigest : public HashFunction<uint32_t, LittleEndian32>
{
protected:
   explicit MessageDigest(const uint8_t in_block_length) : HashFunction(in_block_length) {}
   virtual ~MessageDigest() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
};

class MD2 : public MessageDigest
{
public:
   MD2() : MessageDigest(16) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static constexpr uint8_t digits_of_pi[256] = {
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
   BytesContainer checksum = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   
   void process(const BytesContainer &data, BytesContainer &hash, const uint64_t &index);
   const BytesContainer appendPadding(const BytesContainer &data) const;
};

class MD4 : public MessageDigest
{
public:
   MD4() : MessageDigest(64) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static constexpr uint8_t left_rotation_table[48] = {
      3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19, 3, 7, 11, 19,
      3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13, 3, 5, 9, 13,
      3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15, 3, 9, 11, 15
   };
   
   static constexpr uint8_t word_indexes[48] = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
      0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15,
      0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15
   };
   
   static constexpr uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   static constexpr uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }
   
   static constexpr uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
};

class MD5 : public MessageDigest
{
public:
   MD5() : MessageDigest(64) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static constexpr uint8_t left_rotation_table[64] = {
      7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
      5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
   };

   /* Constants obtained from the formula k(i) = sin(i + 1) * 2^32 for i = 0,...,63. */
   static constexpr uint32_t sine_magic_numbers[64] = {
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
   
   static constexpr uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   static constexpr uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & z) | (y & ~z);
   }
   
   static constexpr uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
   
   static constexpr uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return y ^ (x | ~z);
   }
};

#endif