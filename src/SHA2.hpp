/*
 * Source : http://csrc.nist.gov
 */
#ifndef SHA2_HPP
#define SHA2_HPP

#include "MerkleDamgardFunction.hpp"
#include "Endian.hpp"
#include "Bits.hpp"
#include "Vector.hpp"

#include <array>

template <typename Integer>
struct RoundConstants
{
   static const Vector<Integer> CubicRootPrimes;
   static const std::array<uint8_t, 6> Shifters;
   static const std::array<uint8_t, 6> A;
};

template <typename DataType, uint8_t InputBlockSize>
class SHA2 : public MerkleDamgardFunction<DataType, Endian<BigEndian<DataType>, DataType>, BigEndian64, InputBlockSize>
{
private:
   using MerkleDamgardFunctionType = MerkleDamgardFunction<DataType, Endian<BigEndian<DataType>, DataType>, BigEndian64, InputBlockSize>;
   using DataTypeVector = typename MerkleDamgardFunctionType::DataTypeVector;
   using RC = RoundConstants<DataType>;   
   
protected:      
   SHA2(const DataTypeVector &IV, const uint8_t rounds, const uint8_t output_size) 
      : MerkleDamgardFunctionType(IV, rounds, output_size) {}

   virtual ~SHA2() {}
   
   void compress(DataTypeVector &int_block, DataTypeVector &state) override
   {
      int_block.resize(this->rounds);

      // Extention of the 32-bit 16 blocks in 64 blocks of 32 bits.
      extendWords(int_block, RC::Shifters);

      DataTypeVector hash(state);
      for (uint8_t j = 0; j < this->rounds; ++j)
      {
         const DataType tmp1 = hash[7] + A(hash[4], RC::A[0], RC::A[1], RC::A[2]) + ch(hash[4], hash[5], hash[6]) 
            + RC::CubicRootPrimes[j] + int_block[j];
         const DataType tmp2 = A(hash[0], RC::A[3], RC::A[4], RC::A[5]) + maj(hash[0], hash[1], hash[2]);
         swapHash(hash, tmp1, tmp2);
      }

      this->applyDaviesMayerFunction(hash, state);
   }
   
   /* Extend the 16 words vector to 'rounds' words with specific operations. */
   void extendWords(DataTypeVector &words, const std::array<uint8_t, 6> &to_shift) const
   {
      for (uint8_t j = 16; j < this->rounds; ++j)
      {
         words[j] = words[j - 16] + B(words[j - 15], to_shift[0], to_shift[1], to_shift[2])
                 + words[j - 7] + B(words[j - 2], to_shift[3], to_shift[4], to_shift[5]);
      }
   }
   
   static void swapHash(DataTypeVector &hash, const DataType &tmp1, const DataType &tmp2)
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
   
   static constexpr DataType maj(const DataType &x, const DataType &y, const DataType &z)
   {
      return (x & y) ^ (x & z) ^ (y & z);
   }

   static constexpr DataType ch(const DataType &x, const DataType &y, const DataType &z)
   {
      return (x & y) ^ (~x & z);
   }

   static constexpr DataType A(const DataType &hash, const uint8_t x, const uint8_t y, const uint8_t z)
   {
      return Bits::rotateRight(hash, x, sizeof(DataType) << 3) 
              ^ Bits::rotateRight(hash, y, sizeof(DataType) << 3) 
              ^ Bits::rotateRight(hash, z, sizeof(DataType) << 3);
   }

   static constexpr DataType B(const DataType &word, const uint8_t x, const uint8_t y, const uint8_t z)
   {
      return Bits::rotateRight(word, x, sizeof(DataType) << 3) 
              ^ Bits::rotateRight(word, y, sizeof(DataType) << 3) 
              ^ (word >> z);
   }
};


class SHA224 : public SHA2<uint32_t, 64>
{
public:
   SHA224() 
      : SHA2({0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939, 
              0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4}, 64, 28) {}
};

class SHA256 : public SHA2<uint32_t, 64>
{
public:
   SHA256() 
      : SHA2({0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 
              0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19}, 64, 32) {}
};

class SHA384 : public SHA2<uint64_t, 128>
{
public:
   SHA384() : SHA2({
      0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17, 0x152fecd8f70e5939,
      0x67332667ffc00b31, 0x8eb44a8768581511, 0xdb0c2e0d64f98fa7, 0x47b5481dbefa4fa4}, 80, 48) {}
};

class SHA512 : public SHA2<uint64_t, 128>
{
public:
   SHA512() : SHA2({
      0x6A09E667F3BCC908, 0xBB67AE8584CAA73B, 0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
      0x510E527FADE682D1, 0x9B05688C2B3E6C1F, 0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179}, 80, 64) {}
};

class SHA512_t : public SHA2<uint64_t, 128>
{
protected:
   SHA512_t(const uint8_t output_size, const BytesVector &code) 
      : SHA2({}, 80, output_size) { makeNewIV(code); }
   virtual ~SHA512_t() {}
   
   /* Make a new IV for SHA512 and use it to generate a trucated output. */
   void makeNewIV(const BytesVector &code);
};

class SHA512_224 : public SHA512_t
{
public:
   SHA512_224() : SHA512_t(28, {0x32, 0x32, 0x34}) {}  // code = "224"
   
private:
   BytesVector getOutput(const UInt64Vector &hash) const override;
};

class SHA512_256 : public SHA512_t
{
public:
   SHA512_256() : SHA512_t(32, {0x32, 0x35, 0x36}) {}  // code = "256"
};

#endif