
#ifndef BLAKE_HPP
#define	BLAKE_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"

template <class UInt>
class Blake : public HashFunction<UInt, BigEndian<UInt>>
{   
protected:
   typedef typename HashFunction<UInt, BigEndian<UInt>>::UIntContainer UIntContainer;
   typedef typename HashFunction<UInt, BigEndian<UInt>>::BytesContainer BytesContainer;
   
   /* Default constructor : no salt provided. */
   Blake(const UIntContainer &state, const uint8_t in_block_length) 
      : HashFunction<UInt, BigEndian<UInt>>(in_block_length), IV(state), salt({0, 0, 0, 0}) {}
   
      /* Constructor with a salt provided. */
   Blake(const UIntContainer &state, const UIntContainer &salt, const uint8_t in_block_length) 
      : HashFunction<UInt, BigEndian<UInt>>(in_block_length), IV(state), salt(salt) {}
   
   virtual ~Blake() {}
   
   virtual const BytesContainer encode(const BytesContainer &data) = 0;
   virtual void G(UInt &a, UInt &b, UInt &c, UInt &d, const UIntContainer &block, const uint8_t r, const uint8_t i) const = 0;
   
   const UIntContainer initialize(const UIntContainer &h, const UInt *C)
   {
      UIntContainer v;
      v.reserve(16);
      
      v.insert(v.end(), h.begin(), h.end());
      for(uint8_t i = 0; i < 4; ++i)
      {
         v.push_back(salt[i] ^ C[i]);
      }
      
      v.push_back((counter & 0xFFFFFFFF) ^ C[4]);
      v.push_back((counter & 0xFFFFFFFF) ^ C[5]);
      v.push_back((counter >> 32) ^ C[6]);
      v.push_back((counter >> 32) ^ C[7]);
      
      return v;
   }
   
   void finalize(UIntContainer &h, const UIntContainer &v) const
   {
      for(uint8_t i = 0; i < 8; ++i)
      {
         h[i] ^= v[i] ^ salt[i & 3] ^ v[i + 8];
      }
   }
   
   const UIntContainer IV;
   const UIntContainer salt;
   uint64_t counter;
   
   static constexpr uint8_t sigma[10][16] = {
      {0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
		{14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3},
		{11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4},
      {7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8},
      {9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13},
      {2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9},
		{12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11},
		{13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10},
      {6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5},
		{10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0}
	};
};

class Blake32Bits : public Blake<uint32_t>
{
protected:
   Blake32Bits(const UInt32Container &state, const uint8_t output_size) : Blake(state, 64), output_size(output_size) {}
   virtual ~Blake32Bits() {}
   
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   virtual void G(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, 
           const UInt32Container &block, const uint8_t r, const uint8_t i) const final;
   
   static constexpr uint32_t C[16] = {
      0x243F6A88, 0x85A308D3, 0x13198A2E, 0x03707344,
		0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89,
		0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C,
		0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917
   };
   
   /* Number of 32-bit integers to be outputed. */
   const uint8_t output_size;
   
   static constexpr uint8_t number_of_rounds = 14;
   
   static constexpr uint8_t G_rotate[4] = { 16, 12, 8, 7 };
};

class Blake64Bits : public Blake<uint64_t>
{
protected:
   Blake64Bits(const UInt64Container &state, const uint8_t output_size) : Blake(state, 128), output_size(output_size) {}
   virtual ~Blake64Bits() {}
   
public:
   virtual const BytesContainer encode(const BytesContainer &data) final;
   
private:
   virtual void G(uint64_t &a, uint64_t &b, uint64_t &c, uint64_t &d, 
           const UInt64Container &block, const uint8_t r, const uint8_t i) const final;
   
   static constexpr uint64_t C[16] = {
      0x243F6A8885A308D3, 0x13198A2E03707344,
		0xA4093822299F31D0, 0x082EFA98EC4E6C89,
		0x452821E638D01377, 0xBE5466CF34E90C6C,
		0xC0AC29B7C97C50DD, 0x3F84D5B5B5470917,
		0x9216D5D98979FB1B, 0xD1310BA698DFB5AC,
		0x2FFD72DBD01ADFB7, 0xB8E1AFED6A267E96,
		0xBA7C9045F12C7F99, 0x24A19947B3916CF7,
		0x0801F2E2858EFC16, 0x636920D871574E69
   };
   
   /* Number of 32-bit integers to be outputed. */
   const uint8_t output_size;
   
   static constexpr uint8_t number_of_rounds = 16;
   
   static constexpr uint8_t G_rotate[4] = { 32, 25, 16, 11 };
};

class Blake224 : public Blake32Bits
{
public:
   Blake224() : Blake32Bits({0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939,
		0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4}, 7) {}
};

class Blake256 : public Blake32Bits
{
public:
   Blake256() : Blake32Bits({0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
		0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19}, 8) {}
      
private:
   virtual const BytesContainer appendPadding(const BytesContainer &message) const final;
};

class Blake384 : public Blake64Bits
{
public:
   Blake384() : Blake64Bits({0xCBBB9D5DC1059ED8, 0x629A292A367CD507, 0x9159015A3070DD17, 0x152FECD8F70E5939,
		0x67332667FFC00B31, 0x8EB44A8768581511, 0xDB0C2E0D64F98FA7, 0x47B5481DBEFA4FA4}, 12) {}   
};

class Blake512 : public Blake64Bits
{
public:
   Blake512() : Blake64Bits({0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
		0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179}, 16) {}
      
private:
   virtual const BytesContainer appendPadding(const BytesContainer &message) const final;
};

#endif