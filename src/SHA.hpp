/*
 * Source : http://csrc.nist.gov
 * Requirements : Need of big integers to support length of (2^{128} - 1).
 */
#ifndef SHA_HPP
#define	SHA_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"

#include "Tools.hpp"

template <class UInt>
class SHA : public HashFunction<UInt, BigEndian<UInt> >
{
protected:   
   typedef typename HashFunction<UInt, BigEndian<UInt> >::BytesContainer BytesContainer;
   typedef typename HashFunction<UInt, BigEndian<UInt> >::UIntContainer UIntContainer;
   
   explicit SHA(const UIntContainer &state) : IV(state) {}
   virtual ~SHA() {}
   
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
   virtual const BytesContainer process(const BytesContainer &data, const uint8_t truncate_to) = 0;
   
   /* Extend the 16 words vector to 'max_size' words with specific operations. */
   static void extendWords(UIntContainer &words, const uint8_t max_size, const BytesContainer &to_shift)
   {
      for (uint8_t j = 16; j < max_size; ++j)
      {
         words[j] = words[j - 16] + B(words[j - 15], to_shift[0], to_shift[1], to_shift[2])
                 + words[j - 7] + B(words[j - 2], to_shift[3], to_shift[4], to_shift[5]);
      }
   }
   
   void swapHash(UIntContainer &hash, const UInt &tmp1, const UInt &tmp2)
   {
      hash[7] = hash[6];
      hash[6] = hash[5];
      hash[5] = hash[4];
      hash[4] = hash[3] + tmp1;
      hash[3] = hash[2];
      hash[2] = hash[1];
      hash[1] = hash[0];
      hash[0] = tmp1 + tmp2;
   }
   
   /* Sum in IV the hashes vector. */
   void sumHash(const UIntContainer &hash)
   {
      const uint8_t hash_size = hash.size();
      for (uint8_t j = 0; j < hash_size; ++j)
      {
         IV[j] += hash[j];
      }
   }

   inline static UInt maj(const UInt &x, const UInt &y, const UInt &z)
   {
      return (x & y) ^ (x & z) ^ (y & z);
   }

   inline static UInt ch(const UInt &x, const UInt &y, const UInt &z)
   {
      return (x & y) ^ (~x & z);
   }

   inline static UInt A(const UInt &hash, const uint8_t x, const uint8_t y, const uint8_t z)
   {
      const uint8_t UInt_size = sizeof(UInt) << 3;
      return rotateRight(hash, x, UInt_size) ^ rotateRight(hash, y, UInt_size) ^ rotateRight(hash, z, UInt_size);
   }

   inline static UInt B(const UInt &word, const uint8_t x, const uint8_t y, const uint8_t z)
   {
      const uint8_t UInt_size = sizeof(UInt) << 3;
      return rotateRight(word, x, UInt_size) ^ rotateRight(word, y, UInt_size) ^ (word >> z);
   }
   
   UIntContainer IV;
};

/* Abstract class for SHA algorithm that uses only 32 bits blocks to encode. */
class SHA32Bits : public SHA<uint32_t>
{
public:
   virtual ~SHA32Bits() {}
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   
protected:
   typedef typename SHA<uint32_t>::WordsContainer WordsContainer;
   
   explicit SHA32Bits(const WordsContainer &state) : SHA(state) {}
   virtual const BytesContainer process(const BytesContainer &data, const uint8_t truncate_to) final;
   
private:
   static const uint32_t round_constants[64];
};

/* Abstract class for SHA algorithm that uses only 64 bits blocks to encode. */
class SHA64Bits : public SHA<uint64_t>
{ 
public:
   virtual ~SHA64Bits() {}
   virtual const BytesContainer encode(const BytesContainer &) = 0;

protected:
   typedef typename SHA<uint64_t>::DWordsContainer DWordsContainer;
   
   explicit SHA64Bits(const DWordsContainer &state) : SHA(state) {}
   virtual const BytesContainer process(const BytesContainer &data, const uint8_t truncate_to) final;
  
private:
   /* Derived from the fractional parts of the cube roots of the first eighty primes. */
   static const uint64_t first_cubic_root_primes[80];
};

class SHA1 : public SHA32Bits
{
public:
   SHA1() : SHA32Bits({0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0}) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   static void extendWords(WordsContainer &words, const uint8_t max_size);

   inline static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }

   inline static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
};

class SHA224 : public SHA32Bits
{
public:
   SHA224() : SHA32Bits({0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4}) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class SHA256 : public SHA32Bits
{
public:
   SHA256() : SHA32Bits({0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19}) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class SHA384 : public SHA64Bits
{
public:
   SHA384() : SHA64Bits({
      0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
      0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
   }) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;
};

class SHA512 : public SHA64Bits
{
public:
   SHA512() : SHA64Bits({
      0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
      0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179
   }) {}
   virtual const BytesContainer encode(const BytesContainer &data) final;

   inline const DWordsContainer getIV()
   {
      return IV;
   }

   inline void setIV(const DWordsContainer &IV)
   {
      this->IV = IV;
   }
};

class SHA512_t : public SHA64Bits
{
public:
   SHA512_t() : SHA64Bits({}) {}
   virtual ~SHA512_t() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;

protected:
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