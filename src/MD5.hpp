/*
 * Source : http://tools.ietf.org/html/rfc1321
 */
#ifndef MD5_HPP
#define	MD5_HPP

#include "HashFunction.hpp"

#include <vector>

class MD5 : public HashFunction
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:   
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z);
   static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z);

   static const BytesContainer left_rotation_table;

   /* Constants obtained from the formula k(i) = sin(i + 1) * 2^32 for i = 0,...,63. */
   static const WordsContainer sine_magic_numbers;
};

#endif