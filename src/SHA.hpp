/*
 * Source : http://csrc.nist.gov
 * TO DO : Remove duplicate code with template for uint32_t and uint64_t.
 * Requirements : Need of big integers to support length of (2^{128} - 1).
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
   const BytesContainer process512(const BytesContainer &data, DWordsContainer &IV, const uint8_t truncate_to);
   
   const BytesContainer appendDWordPadding(const BytesContainer &data) const;
   static const DWordsContainer getBigEndianDWordBlocks(const BytesContainer &bytes, const uint64_t &block_index);
   static const BytesContainer getBigEndianDWordOutput(const uint16_t max_words, const DWordsContainer &hash);
   
   static uint64_t maj(const uint64_t &x, const uint64_t &y, const uint64_t &z);
   static uint64_t ch(const uint64_t &x, const uint64_t &y, const uint64_t &z);
   static uint64_t A(const uint64_t &hash, const uint64_t &x, const uint64_t &y, const uint64_t &z);
   static uint64_t B(const uint64_t &word, const uint64_t &x, const uint64_t &y, const uint64_t &z);
   
   static const WordsContainer round_constants;
   
   /* Derived from the fractional parts of the cube roots of the first eighty primes. */
   static const DWordsContainer first_cubic_root_primes;
};

class SHA1 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   WordsContainer IV = {
      0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0
   };
   
   virtual void extendWords(WordsContainer &words, const uint8_t max_size) final;
   
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
   
private:
   WordsContainer IV = {
      0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
      0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
   };
};

class SHA256 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   WordsContainer IV = {
      0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
      0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
   };
};

class SHA384 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   DWordsContainer IV = {
      0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
      0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
   };
};

class SHA512 : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
   const DWordsContainer getIV() { return IV; }
   void setIV(const DWordsContainer &IV) { this->IV = IV; }
   
private:
   DWordsContainer IV = {
      0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
      0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179
   };
};

class SHA512_t : public SHA
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
   
protected: 
   DWordsContainer IV;
   void buildIV(const BytesContainer &t);
};

class SHA512_224 : public SHA512_t
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class SHA512_256 : public SHA512_t
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

#endif