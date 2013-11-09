/*
 * Source : http://csrc.nist.gov
 */
#ifndef SHA1_HPP
#define SHA1_HPP

#include "MerkleDamgardFunction.hpp"
#include "BigEndian.hpp"

#include <array>

class SHA1 : public MerkleDamgardFunction<uint32_t, BigEndian32, BigEndian64, 64>
{
public:
   SHA1() 
      : MerkleDamgardFunction({0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0}, 80, 20) {}
   
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
   
   //static constexpr uint8_t rounds = 80;
   
   /* Magic constants for the 80 rounds processing. */
   static constexpr std::array<uint32_t, 4> k = {{0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6}};
};

#endif