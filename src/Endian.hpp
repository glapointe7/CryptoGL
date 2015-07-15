/*
 * Uses Curiously recurring template design pattern to simulate static polymorphism.
 */
#ifndef ENDIAN_HPP
#define ENDIAN_HPP

#include "Types.hpp"

namespace CryptoGL
{
    template <class Endianess, typename UInteger>
    class Endian
    {
    static_assert(std::is_integral<UInteger>::value, "Endian: The type 'UInteger' must be an integral type.");
    using UIntVector = Vector<UInteger>;

    public:
       static UInteger toInteger(const BytesVector &bytes)
       {
          return Endianess::toInteger(bytes);
       }

       static BytesVector toBytesVector(const UInteger &value)
       {
          return Endianess::toBytesVector(value);
       }

       /* Convert a vector to an integer from a range(from, to). */
       static UInteger toIntegerRange(const BytesVector &bytes, const uint64_t &from, const uint64_t &to)
       {
          return toInteger(bytes.range(from, to));
       }

       /* Convert a vector to an integer from a range(from, bytes.end()). */
       static UInteger toIntegerRange(const BytesVector &bytes, const uint64_t &from)
       {
          return toInteger(bytes.range(from));
       }

       /* Convert a vector of bytes to a vector of integers with the proper endianness. */
       static UIntVector toIntegersVector(const BytesVector &V)
       {
          const uint8_t V_size = V.size();
          const uint8_t type_size = sizeof(UInteger);
          UIntVector result;
          result.reserve(V_size / type_size);
          for(uint8_t i = 0; i < V_size; i += type_size)
          {
             result.push_back(toIntegerRange(V, i, i + type_size));
          }

          return result;
       }

       /* Convert a vector of bytes to a vector of integers for a specific range :
        * start : starting position in the vector 'V'
        * length : number of bytes from 'V' at 'start' to be converted.
        */
       static UIntVector toIntegersVector(const BytesVector &V, const uint64_t &start, const uint64_t &length)
       {
          const uint8_t type_size = sizeof(UInteger);
          UIntVector result;
          result.reserve(length / type_size);
          for(uint8_t i = 0; i < length; i += type_size)
          {
             const uint64_t j = i + start;
             result.push_back(toIntegerRange(V, j, j + type_size));
          }

          return result;
       }

       /* Convert a vector of integers to a vector of bytes with the proper endianness. */
       static BytesVector toBytesVector(const UIntVector &V)
       {
          const uint8_t BlockSize = V.size();
          BytesVector result;
          result.reserve(BlockSize * sizeof(UInteger));
          for(uint8_t i = 0; i < BlockSize; ++i)
          {
             result.extend(toBytesVector(V[i]));
          }

          return result;
       }

       /* Convert a vector of integers to a vector of bytes with the proper endianness with. 
          a specific 'length' of integers to take from 'V'. */
       static BytesVector toBytesVector(const UIntVector &V, const uint64_t &length)
       {
          BytesVector result;
          result.reserve(length * sizeof(UInteger));
          for(uint8_t i = 0; i < length; ++i)
          {
             result.extend(toBytesVector(V[i]));
          }

          return result;
       }
    };


    template <typename UInteger>
    class LittleEndian : public Endian<LittleEndian<UInteger>, UInteger>
    {
    public:   
       /* Convert an integer 'value' to a bytes vector in Little-Endian. */
       static BytesVector toBytesVector(const UInteger &value)
       {
          BytesVector bytes;
          const uint8_t size_UInt_8 = sizeof(UInteger) * 8;
          bytes.reserve(sizeof(UInteger));
          for(uint8_t i = 0; i < size_UInt_8; i += 8)
          {
             bytes.push_back((value >> i) & 0xFF);
          }

          return bytes;
       }

       /* Convert a bytes vector 'bytes' to an integer value in Little-Endian. */
       static UInteger toInteger(const BytesVector &bytes)
       {
          UInteger value = 0;
          for(uint8_t i = 0; i < sizeof(UInteger); ++i)
          {
             value |= (static_cast<UInteger>(bytes[i]) << (i * 8));
          }

          return value; 
       }
    };

    template <typename UInteger>
    class BigEndian : public Endian<BigEndian<UInteger>, UInteger>
    {
    public:   
       static UInteger toInteger(const BytesVector &bytes)
       {
          UInteger value = 0;
          for(uint8_t i = 0; i < sizeof(UInteger); ++i)
          {
             value |= (static_cast<UInteger>(bytes[i]) << ((sizeof(UInteger) - i - 1) * 8));
          }

          return value; 
       }

       static BytesVector toBytesVector(const UInteger &value)
       {
          BytesVector bytes;
          bytes.reserve(sizeof(UInteger));
          for(int8_t i = (sizeof(UInteger) - 1) * 8; i >= 0; i -= 8)
          {
             bytes.push_back((value >> i) & 0xFF);
          }

          return bytes;
       }
    };

    using BigEndian8 = Endian<BigEndian<uint8_t>, uint8_t>;
    using BigEndian16 = Endian<BigEndian<uint16_t>, uint16_t>;
    using BigEndian32 = Endian<BigEndian<uint32_t>, uint32_t>;
    using BigEndian64 = Endian<BigEndian<uint64_t>, uint64_t>;

    using LittleEndian8 = Endian<LittleEndian<uint8_t>, uint8_t>;
    using LittleEndian16 = Endian<LittleEndian<uint16_t>, uint16_t>;
    using LittleEndian32 = Endian<LittleEndian<uint32_t>, uint32_t>;
    using LittleEndian64 = Endian<LittleEndian<uint64_t>, uint64_t>;
}

#endif