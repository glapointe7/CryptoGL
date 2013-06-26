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
   virtual ~SHA() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
   
protected:
   virtual void extendWords(WordsContainer &words, const uint8_t max_size);
   void extendDWords(DWordsContainer &dwords, const uint8_t max_size);
   const BytesContainer process(const BytesContainer &data, WordsContainer &IV, const uint8_t truncate_to);
   
   const BytesContainer appendDWordPadding(const BytesContainer &data) const;
   
   static const DWordsContainer getBigEndianDWordBlocks(const BytesContainer &bytes, const uint64_t &block_index);
   static const BytesContainer getBigEndianDWordOutput(const uint16_t max_words, const DWordsContainer &hash);
   
   static const WordsContainer round_constants;
   
   /* Derived from the fractional parts of the cube roots of the first eighty primes. */
   static const DWordsContainer first_primes;
};

class SHA1 : public SHA
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

class SHA512 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

#endif