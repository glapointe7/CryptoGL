/*
 * Encryptage SHA1 (http://www.itl.nist.gov/fipspubs/fip180-1.htm)
 */
#ifndef SHA1_HPP
#define	SHA1_HPP

#include "HashFunctions.hpp"

class SHA1 : public HashFunctions
{
public:
   virtual BytesContainer encode(const BytesContainer &);
   
private:
   virtual BitsContainer addPadding(const BitsContainer &);
   
   // variables d'état initialisées en LITTLE ENDIAN.
   WordsContainer state = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0};
   
   static uint32_t F(uint32_t x, uint32_t y, uint32_t z);
   static uint32_t G(uint32_t x, uint32_t y, uint32_t z);
   static uint32_t H(uint32_t x, uint32_t y, uint32_t z);
};

#endif