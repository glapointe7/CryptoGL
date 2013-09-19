/*
 * Source : https://131002.net/blake/blake.pdf
 */
#ifndef BLAKE_HPP
#define BLAKE_HPP

#include "HashFunction.hpp"
#include "BigEndian.hpp"
#include "Bits.hpp"

/*
 * Since we cannot override static members like constants for 32 and 64 bits integer, 
 * we have to define specialized templated classes that will return our constants.
 */
template <class UInt64Type>
class ConstantsGetter
{
public:
   static const std::vector<UInt64Type> G_constants()
   {
      return {
         0x243F6A8885A308D3, 0x13198A2E03707344, 0xA4093822299F31D0, 0x082EFA98EC4E6C89,
         0x452821E638D01377, 0xBE5466CF34E90C6C, 0xC0AC29B7C97C50DD, 0x3F84D5B5B5470917,
         0x9216D5D98979FB1B, 0xD1310BA698DFB5AC, 0x2FFD72DBD01ADFB7, 0xB8E1AFED6A267E96,
         0xBA7C9045F12C7F99, 0x24A19947B3916CF7, 0x0801F2E2858EFC16, 0x636920D871574E69
      };
   }
   
   static const UInt64Type G_shifts(const UInt64Type &value, const uint8_t index)
   {
      constexpr uint8_t shifts[4] = {32, 25, 16, 11};
      return Bits::rotateRight64(value, shifts[index]);
   }
};

template<>
class ConstantsGetter<uint32_t>
{
public:
   static const UInt32Vector G_constants()
   {
      return {
         0x243F6A88, 0x85A308D3, 0x13198A2E, 0x03707344,
         0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89,
         0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C,
         0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917
      };
   }
   
   static const uint32_t G_shifts(const uint32_t &value, const uint8_t index)
   {
      constexpr uint8_t shifts[4] = {16, 12, 8, 7};
      return Bits::rotateRight(value, shifts[index]);
   }
};



template <class DataType, uint8_t InputBlockSize>
class Blake : public HashFunction<DataType, BigEndian<DataType>>
{
static_assert(!(InputBlockSize & 0x3F), "'InputBlockSize' has to be a multiple of 64.");   
   
private:
   using HashFunctionType = HashFunction<DataType, BigEndian<DataType>>;
   using DataTypeVector = typename HashFunctionType::DataTypeVector;
   
   const DataTypeVector salt;
   uint64_t counter;
   
   const uint8_t rounds;
   
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
   
   void G(DataType &a, DataType &b, DataType &c, DataType &d, 
           const DataTypeVector &block, const uint8_t r, const uint8_t i) const
   {
      const DataTypeVector C = ConstantsGetter<DataType>::G_constants();
      a += b + (block[sigma[r % 10][i]] ^ C[sigma[r % 10][i + 1]]);
      d = ConstantsGetter<DataType>::G_shifts(d ^ a, 0);
      c += d;
      b = ConstantsGetter<DataType>::G_shifts(b ^ c, 1);

      a += b + (block[sigma[r % 10][i + 1]] ^ C[sigma[r % 10][i]]);
      d = ConstantsGetter<DataType>::G_shifts(d ^ a, 2);
      c += d;
      b = ConstantsGetter<DataType>::G_shifts(b ^ c, 3);
   }
      
   virtual void compress(DataTypeVector &int_block, DataTypeVector &hash) final
   {
      DataTypeVector V = this->initialize(hash, ConstantsGetter<DataType>::G_constants());
      for(uint8_t j = 0; j < rounds; ++j)
      {
         this->G(V[0], V[4], V[8], V[12], int_block, j, 0);
         this->G(V[1], V[5], V[9], V[13], int_block, j, 2);
         this->G(V[2], V[6], V[10], V[14], int_block, j, 4);
         this->G(V[3], V[7], V[11], V[15], int_block, j, 6);
         this->G(V[0], V[5], V[10], V[15], int_block, j, 8);
         this->G(V[1], V[6], V[11], V[12], int_block, j, 10);
         this->G(V[2], V[7], V[8], V[13], int_block, j, 12);
         this->G(V[3], V[4], V[9], V[14], int_block, j, 14);
      }

      this->finalize(hash, V);
   }
   
   const DataTypeVector initialize(const DataTypeVector &h, const DataTypeVector &C)
   {
      DataTypeVector v;
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
   
   void setCounter(const uint64_t &data_size, const uint64_t &bytes_len)
   {
      const uint8_t rest = data_size % InputBlockSize;
      if(rest == 0 || rest >= InputBlockSize - (sizeof(DataType) << 1))
      {
         counter = 0;
      }
      else
      {
         counter = (InputBlockSize - (bytes_len - data_size)) << 3;
      }
   }
   
   void finalize(DataTypeVector &h, const DataTypeVector &v) const
   {
      for(uint8_t i = 0; i < 8; ++i)
      {
         h[i] ^= v[i] ^ salt[i & 3] ^ v[i + 8];
      }
   }
   
protected:
   /* Constructor with a salt provided. */
   Blake(const DataTypeVector &IV, const DataTypeVector &salt, const uint8_t rounds, const uint8_t output_size) 
      : HashFunctionType(InputBlockSize, output_size, IV), 
           salt(salt), rounds(rounds) {}
      
   /* Default constructor : no salt provided. */
   Blake(const DataTypeVector &IV, const uint8_t rounds, const uint8_t output_size) 
      : Blake(IV, DataTypeVector{0, 0, 0, 0}, rounds, output_size) {}
   
   virtual ~Blake() {} 
   
public:   
   virtual const BytesVector encode(const BytesVector &data) final
   {
      const uint64_t data_size = data.size();
      BytesVector bytes = this->pad(data);
      bytes = this->template appendLength<BigEndian64>(bytes, data_size << 3);

      const uint64_t bytes_len = bytes.size();
      setCounter(data_size, bytes_len);

      DataTypeVector hash(this->getIV());
      for (uint64_t i = 0; i < bytes_len; i += InputBlockSize)
      {              
         DataTypeVector int_block = this->getInputBlocks(bytes, i);    
         compress(int_block, hash);
      }

      return this->getOutput(hash);
   }  
};

class Blake224 : public Blake<uint32_t, 64>
{
public:
   Blake224() : Blake({0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939,
		0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4}, 14, 28) {}
};

class Blake256 : public Blake<uint32_t, 64>
{
public:
   Blake256() : Blake({0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
		0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19}, 14, 32) {}
      
private:
   virtual const BytesVector pad(const BytesVector &message) const final;
};

class Blake384 : public Blake<uint64_t, 128>
{
public:
   Blake384() : Blake({0xCBBB9D5DC1059ED8, 0x629A292A367CD507, 0x9159015A3070DD17, 0x152FECD8F70E5939,
		0x67332667FFC00B31, 0x8EB44A8768581511, 0xDB0C2E0D64F98FA7, 0x47B5481DBEFA4FA4}, 16, 48) {}   
};

class Blake512 : public Blake<uint64_t, 128>
{
public:
   Blake512() : Blake({0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
		0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179}, 16, 64) {}
      
private:
   virtual const BytesVector pad(const BytesVector &message) const final;
};

#endif