/*
 * Source : https://131002.net/blake/blake.pdf
 */
#ifndef BLAKE_HPP
#define BLAKE_HPP

#include "HashFunction.hpp"
#include "Bits.hpp"

#include <array>

namespace CryptoGL
{
    /*
     * Since we cannot override static members like constants for 32 and 64 bits integer, 
     * we have to define specialized templated classes that will return our constants.
     */
    template <typename VectorType>
    struct VectorGetter
    {
       static const VectorType G;
    };

    template <typename Integer, uint8_t Index>
    struct ShiftingGetter
    {
       static constexpr Integer G(const Integer &value) { return value; }
    };

    template <uint8_t Index>
    struct ShiftingGetter<uint64_t, Index>
    {
       static constexpr uint64_t G(const uint64_t &value)
       {
          constexpr std::array<uint8_t, 4> shifts = {{32, 25, 16, 11}};
          return Bits::rotateRight64(value, shifts[Index]);
       }
    };

    template <uint8_t Index>
    struct ShiftingGetter<uint32_t, Index>
    {
       static constexpr uint32_t G(const uint32_t &value)
       {
          constexpr std::array<uint8_t, 4> shifts = {{16, 12, 8, 7}};
          return Bits::rotateRight(value, shifts[Index]);
       }
    };


    template <class DataType, uint8_t InputBlockSize>
    class Blake : public HashFunction<DataType, Endian<BigEndian<DataType>, DataType>>
    {
    static_assert(!(InputBlockSize % 64), "'InputBlockSize' has to be a multiple of 64.");   

    private:
       using HashFunctionType = HashFunction<DataType, Endian<BigEndian<DataType>, DataType>>;
       using DataTypeVector = typename HashFunctionType::DataTypeVector;
       using GVector = VectorGetter<DataTypeVector>;

       const DataTypeVector salt;
       uint64_t counter;

       const uint8_t rounds;
       
       DataTypeVector registers;

       static constexpr std::array<std::array<uint8_t, 16>, 10> sigma = {{
           {{0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15}},
           {{14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3}},
           {{11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4}},
           {{7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8}},
           {{9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13}},
           {{2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9}},
           {{12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11}},
           {{13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10}},
           {{6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5}},
           {{10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13,  0}}
       }};

       void G(const std::array<uint8_t, 4> &I, const uint8_t k, const uint8_t i)
       {
          const DataTypeVector C = GVector::G;
          registers[I[0]] += registers[I[1]] + (this->current_block[sigma[k][i]] ^ C[sigma[k][i + 1]]);
          registers[I[3]] = ShiftingGetter<DataType, 0>::G(registers[I[3]] ^ registers[I[0]]);
          registers[I[2]] += registers[I[3]];
          registers[I[1]] = ShiftingGetter<DataType, 1>::G(registers[I[1]] ^ registers[I[2]]);

          registers[I[0]] += registers[I[1]] + (this->current_block[sigma[k][i + 1]] ^ C[sigma[k][i]]);
          registers[I[3]] = ShiftingGetter<DataType, 2>::G(registers[I[3]] ^ registers[I[0]]);
          registers[I[2]] += registers[I[3]];
          registers[I[1]] = ShiftingGetter<DataType, 3>::G(registers[I[1]] ^ registers[I[2]]);
       }

       void compress(DataTypeVector &hash) override
       {
          this->initialize(hash, GVector::G);
          for(uint8_t j = 0; j < rounds; ++j)
          {
              const uint8_t k = j % 10;
              for(uint8_t i = 0; i < 4; ++i)
              {
                  const std::array<uint8_t, 4> Indices = {{i, static_cast<uint8_t>(i + 4), static_cast<uint8_t>(i + 8), static_cast<uint8_t>(i + 12)}};
                  G(Indices, k, 2 * i);
              }
             
             G({{0, 5, 10, 15}}, k, 8);
             G({{1, 6, 11, 12}}, k, 10);
             G({{2, 7, 8, 13}}, k, 12);
             G({{3, 4, 9, 14}}, k, 14);
          }

          this->finalize(hash);
       }

       void initialize(const DataTypeVector &hash, const DataTypeVector &C)
       {
           registers.reserve(16);
           registers.extend(hash);
           for(uint8_t i = 0; i < 4; ++i)
           {
             registers.push_back(salt[i] ^ C[i]);
           }

           registers.push_back((counter & 0xFFFFFFFF) ^ C[4]);
           registers.push_back((counter & 0xFFFFFFFF) ^ C[5]);
           registers.push_back((counter >> 32) ^ C[6]);
           registers.push_back((counter >> 32) ^ C[7]);
       }

       void setCounter(const uint64_t &data_size, const uint64_t &bytes_len)
       {
          const uint8_t rest = data_size % InputBlockSize;
          if(rest == 0 || rest >= InputBlockSize - (sizeof(DataType) * 2))
          {
             counter = 0;
          }
          else
          {
             counter = (InputBlockSize - (bytes_len - data_size)) * 8;
          }
       }

       void finalize(DataTypeVector &h) const
       {
          for(uint8_t i = 0; i < 8; ++i)
          {
             h[i] ^= registers[i] ^ salt[i % 4] ^ registers[i + 8];
          }
       }

    protected:
       /* Constructor with a salt provided. */
       Blake(const DataTypeVector &IV, const DataTypeVector &salt, const uint8_t rounds, const uint8_t output_size) 
          : HashFunctionType(InputBlockSize, output_size, IV), 
               salt(salt), rounds(rounds) {}

       /* Default constructor : no salt provided. */
       Blake(const DataTypeVector &IV, const uint8_t rounds, const uint8_t output_size) 
          : Blake(IV, DataTypeVector(4, 0), rounds, output_size) {}

       virtual ~Blake() {} 

    public:   
       BytesVector encode(const BytesVector &message) override
       {
          const uint64_t message_size = message.size();
          BytesVector bytes = this->pad(message);
          bytes = this->template appendLength<BigEndian64>(bytes, message_size * 8);

          const uint64_t bytes_len = bytes.size();
          setCounter(message_size, bytes_len);

          DataTypeVector hash(this->getIV());
          for (uint64_t i = 0; i < bytes_len; i += InputBlockSize)
          {              
             this->current_block = this->convertToIntegersBlock(bytes, i);    
             compress(hash);
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
       BytesVector pad(BytesVector message) const override;
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
       BytesVector pad(BytesVector message) const override;
    };
}

#endif