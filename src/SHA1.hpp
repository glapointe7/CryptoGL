/*
 * Hash SHA1 (http://www.itl.nist.gov/fipspubs/fip180-1.htm)
 */
#ifndef SHA1_HPP
#define	SHA1_HPP

#include "HashFunction.hpp"

class SHA1 : public HashFunction
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:   
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z);
};

#endif