/*
 * Source : http://csrc.nist.gov
 * TO DO : Implement SHA-384, SHA-512, SHA-512/224 and SHA-512/256.
 * Requirements : Need of big integers (2^{128} - 1).
 */
#ifndef SHA_HPP
#define	SHA_HPP

#include "HashFunction.hpp"

class SHA : public HashFunction
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
   
protected:
   virtual void extendWords(WordsContainer &words, const uint8_t max_size);
   
   static const WordsContainer round_constants;
};

class SHA128 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   virtual void extendWords(WordsContainer &words, const uint8_t max_size) final;
   
   static uint32_t F(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (~x & z);
   }
   
   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }
   
   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
};

class SHA224 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class SHA256 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

#endif