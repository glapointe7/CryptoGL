
#ifndef SHA1_HPP
#define	SHA1_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"

#define INPUT_BLOCK_LENGTH 64

class SHA1 : public HashFunction<uint32_t, BigEndian>
{
public:
   SHA1() : HashFunction<uint32_t, BigEndian<uint32_t> >(INPUT_BLOCK_LENGTH) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   static constexpr uint32_t IV[5] = {
      0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0
   };
   
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
};

#endif