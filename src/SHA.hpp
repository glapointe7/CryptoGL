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
   typedef std::vector<UInt> UIntContainer;
   typedef typename HashFunction<UInt, BigEndian<UInt> >::WordsContainer WordsContainer;
   typedef typename HashFunction<UInt, BigEndian<UInt> >::DWordsContainer DWordsContainer;
      
public:
   virtual ~SHA() {}
   virtual const BytesContainer encode(const BytesContainer &data) = 0;

protected:   
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
   
   /*const BytesContainer process(const BytesContainer &data, UIntContainer &IV, const uint8_t truncate_to,
           const BytesContainer &extend_shifts, const BytesContainer &tmp_shifts, const uint8_t extend_to, const UIntContainer &round_const)
   {
      BytesContainer bits(this->appendPadding(data));
      const uint64_t bits_len = bits.size();
      const uint8_t block_size = sizeof(UInt) << 4;

      for (uint64_t i = 0; i < bits_len; i += block_size)
      {
         UIntContainer words = this->getInputBlocks(bits, i);
         words.resize(extend_to);

         // Extention of the 32-bit 16 blocks in 64 or 80 blocks of 32 or 64 bits.
         extendWords(words, extend_to, extend_shifts);

         UIntContainer hash(IV);
         for (uint8_t j = 0; j < extend_to; ++j)
         {
            const UInt tmp1 = hash[7] + A(hash[4], tmp_shifts[0], tmp_shifts[1], tmp_shifts[2]) + 
               ch(hash[4], hash[5], hash[6]) + round_const[j] + words[j];
            const UInt tmp2 = A(hash[0], tmp_shifts[3], tmp_shifts[4], tmp_shifts[5]) + maj(hash[0], hash[1], hash[2]);
            swapHash(hash, tmp1, tmp2);
         }

         for (uint8_t j = 0; j < 8; ++j)
         {
            IV[j] += hash[j];
         }
      }

      return this->getOutput(truncate_to, IV);
   }*/

   const BytesContainer process(const BytesContainer &data, WordsContainer &IV, const uint8_t truncate_to)
   {
      BytesContainer bits(this->appendPadding(data));
      const uint64_t bits_len = bits.size();

      for (uint64_t i = 0; i < bits_len; i += 64)
      {
         WordsContainer words = this->getInputBlocks(bits, i);
         words.resize(64);

         // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
         extendWords(words, 64, {7, 18, 3, 19, 17, 10});

         WordsContainer hash(IV);
         for (uint8_t j = 0; j < 64; ++j)
         {
            const uint32_t tmp1 = hash[7] + A(hash[4], 6, 11, 25) + ch(hash[4], hash[5], hash[6]) + round_constants[j] + words[j];
            const uint32_t tmp2 = A(hash[0], 2, 13, 22) + maj(hash[0], hash[1], hash[2]);
            swapHash(hash, tmp1, tmp2);
         }

         for (uint8_t j = 0; j < 8; ++j)
         {
            IV[j] += hash[j];
         }
      }

      return this->getOutput(truncate_to, IV);
   }

   const BytesContainer process512(const BytesContainer &data, DWordsContainer &IV, const uint8_t truncate_to)
   {
      BytesContainer bits(this->appendPadding(data));
      const uint64_t bits_len = bits.size();

      for (uint64_t i = 0; i < bits_len; i += 128)
      {
         DWordsContainer words = this->getInputBlocks(bits, i);
         words.resize(80);

         // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
         extendWords(words, 80, {1, 8, 7, 19, 61, 6});

         DWordsContainer hash(IV);
         for (uint8_t j = 0; j < 80; ++j)
         {
            const uint64_t tmp1 = hash[7] + A(hash[4], 14, 18, 41) + ch(hash[4], hash[5], hash[6]) + first_cubic_root_primes[j] + words[j];
            const uint64_t tmp2 = A(hash[0], 28, 34, 39) + maj(hash[0], hash[1], hash[2]);
            swapHash(hash, tmp1, tmp2);
         }

         for (uint8_t j = 0; j < 8; ++j)
         {
            IV[j] += hash[j];
         }
      }

      return this->getOutput(truncate_to, IV);
   }

   static UInt maj(const UInt &x, const UInt &y, const UInt &z)
   {
      return (x & y) ^ (x & z) ^ (y & z);
   }

   static UInt ch(const UInt &x, const UInt &y, const UInt &z)
   {
      return (x & y) ^ (~x & z);
   }

   static UInt A(const UInt &hash, const uint8_t x, const uint8_t y, const uint8_t z)
   {
      const uint8_t UInt_size = sizeof(UInt) << 3;
      return rotateRight(hash, x, UInt_size) ^ rotateRight(hash, y, UInt_size) ^ rotateRight(hash, z, UInt_size);
   }

   static UInt B(const UInt &word, const uint8_t x, const uint8_t y, const uint8_t z)
   {
      const uint8_t UInt_size = sizeof(UInt) << 3;
      return rotateRight(word, x, UInt_size) ^ rotateRight(word, y, UInt_size) ^ (word >> z);
   }

   /* Should be static const instead of const. */
   const WordsContainer round_constants = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
   };

   /* Derived from the fractional parts of the cube roots of the first eighty primes. */
   const DWordsContainer first_cubic_root_primes = {
      0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
      0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
      0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
      0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
      0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
      0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
      0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
      0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
      0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
      0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
      0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
      0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
      0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
      0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
      0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
      0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
      0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
      0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
      0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
      0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817
   };
};

class SHA1 : public SHA<uint32_t>
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   WordsContainer IV = {
      0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xC3D2E1F0
   };

   static void extendWords(WordsContainer &words, const uint8_t max_size);

   static uint32_t G(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return (x & y) | (x & z) | (y & z);
   }

   static uint32_t H(const uint32_t x, const uint32_t y, const uint32_t z)
   {
      return x ^ y ^ z;
   }
};

class SHA224 : public SHA<uint32_t>
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   WordsContainer IV = {
      0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
      0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
   };
};

class SHA256 : public SHA<uint32_t>
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   WordsContainer IV = {
      0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
      0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
   };
};

class SHA384 : public SHA<uint64_t>
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

private:
   DWordsContainer IV = {
      0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
      0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4
   };
};

class SHA512 : public SHA<uint64_t>
{
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;

   const DWordsContainer getIV()
   {
      return IV;
   }

   void setIV(const DWordsContainer &IV)
   {
      this->IV = IV;
   }

private:
   DWordsContainer IV = {
      0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
      0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179
   };
};

class SHA512_t : public SHA<uint64_t>
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