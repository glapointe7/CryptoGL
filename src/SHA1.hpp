
#ifndef SHA1_HPP
#define SHA1_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"

class SHA1 : public HashFunction<uint32_t, BigEndian32>
{
public:
   SHA1() : HashFunction(64, 20, {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0}) {}
   virtual const BytesVector encode(const BytesVector &data) final;
   
   void compress(UInt32Vector &int_block, UInt32Vector &state);
   
private:   
   static constexpr uint32_t ch(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) ^ (~x & z);
   }
   
   static constexpr uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }

   static constexpr uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
   
   static constexpr uint8_t rounds = 80;
   
   /* Magic constants for the 80 rounds processing. */
   static constexpr uint32_t k[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};
};

#endif