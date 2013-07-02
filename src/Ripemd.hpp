
#ifndef RIPEMD_HPP
#define	RIPEMD_HPP

#include "HashFunction.hpp"
#include "LittleEndian.hpp"

class Ripemd : public HashFunction<uint32_t, LittleEndian32>
{
protected:
   typedef typename HashFunction<uint32_t, LittleEndian32>::BytesContainer BytesContainer;
   typedef typename HashFunction<uint32_t, LittleEndian32>::WordsContainer WordsContainer;
   
public:
   virtual ~Ripemd() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;

protected:
   explicit Ripemd(const WordsContainer &state) : IV(state) {}
   
   inline static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
   
   inline static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   inline static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x | ~y) ^ z;
   }
   
   inline static uint32_t I(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & z) | (y & ~z);
   }
   
   inline static uint32_t J(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ (y | ~z);
   }
   
   void swapHashWithoutRotate(WordsContainer &hash, const uint32_t tmp);
   void swapHashWithRotate(WordsContainer &hash, const uint32_t tmp);
   
   /* Initial vector. */
   const WordsContainer IV;
   
   static const uint32_t magic_numbers1[5];
   static const uint32_t magic_numbers2[4];
   static const uint32_t magic_numbers_big2[5];
   static const uint8_t word_selection1[80];
   static const uint8_t word_selection2[80];
   static const uint8_t left_shift1[80];
   static const uint8_t left_shift2[80];
};

class Ripemd128 : public Ripemd
{
public:
   Ripemd128() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476}) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Ripemd160 : public Ripemd
{
public:
   Ripemd160() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0}) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Ripemd256 : public Ripemd
{
public:
   Ripemd256() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476,
      0x76543210, 0xFEDCBA98, 0x89ABCDEF, 0x01234567}) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class Ripemd320 : public Ripemd
{
public:
   Ripemd320() : Ripemd({0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0,
      0x76543210, 0xFEDCBA98, 0x89ABCDEF, 0x01234567, 0x3C2D1E0F}) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
};
#endif