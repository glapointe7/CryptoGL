
#ifndef BITS_HPP
#define BITS_HPP

#include "Types.hpp"


namespace Bits
{
   /* Extract the Least Signifiant Bits from value. */
   template <typename UInteger>
   constexpr UInteger LSB(const UInteger &value, const uint8_t bits_to_extract)
   {
      static_assert(std::is_integral<UInteger>::value, "Type UInt must be an integral type.");
      return value & (static_cast<UInteger>(1) << bits_to_extract);
   }

   /* Extract the Most Signifiant Bits from value. */
   template <typename UInteger>
   constexpr UInteger MSB(const UInteger &value, const uint8_t bits_to_extract)
   {
      static_assert(std::is_integral<UInteger>::value, "Type UInt must be an integral type.");
      return value >> ((sizeof(UInteger) * 8) - bits_to_extract);
   }
   
   uint8_t msb(const BytesVector &V);
   
   uint8_t lsb(const BytesVector &V);

   /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,63}. */
   template <typename UInteger>
   constexpr UInteger rotateLeft(const UInteger &value, const uint8_t shift, const uint8_t max = 32)
   {
      static_assert(std::is_integral<UInteger>::value, "Type UInt must be an integral type.");
      return ((value << shift) | (value >> (max - shift))) & ((1ull << max) - 1);
   }
   
   /* Rotate left 'value' of 'shift' bits of a 64-bit value. */
   constexpr uint64_t rotateLeft64(const uint64_t &value, const uint8_t shift)
   {
      return (value << shift) | (value >> (64 - shift));
   }

   /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,63}. */
   template <typename UInteger>
   constexpr UInteger rotateRight(const UInteger &value, const uint8_t shift, const uint8_t max = 32)
   {
      static_assert(std::is_integral<UInteger>::value, "Type UInt must be an integral type.");
      return ((value >> shift) | (value << (max - shift)));
   }
   
   /* Rotate right 'value' of 'shift' bits of a 64-bit value. */
   constexpr uint64_t rotateRight64(const uint64_t &value, const uint8_t shift)
   {
      return ((value >> shift) | (value << (64 - shift)));
   }
   
   /* Rotate left 'shift' bits of a 16-byte vector. */
   BytesVector rotateLeft128(const BytesVector &bytes, const uint8_t shift);
   
   /* Test a bit at a 'pos' in 'value'. */
   template <typename Integer>
   constexpr bool getBitAtPosition(const uint8_t pos, const Integer value)
   {
      static_assert(std::is_integral<Integer>::value, "Type UInt must be an integral type.");
      return (value & (1ull << pos)) > 0;
   }
   
   /* Set a bit at 'pos' in 'value'. */
   template <typename Integer>
   constexpr Integer setBitAtPosition(const uint8_t pos, const Integer value)
   {
      static_assert(std::is_integral<Integer>::value, "Type UInt must be an integral type.");
      return value | (1ull << pos);
   }
   
   /* Number of trailing zeros : return the largest integer 'count' such that 
      2^count divide value. */
   template <typename Integer>
   uint8_t ntz(const Integer value)
   {
      uint8_t count = 0;
      while(!getBitAtPosition(count, value))
      {
         ++count;
      }
      
      return count;
   }
   
   /* Swap endianness of an integer 'value'. ***To move in Endian class.****/
   uint32_t bytesSwap(const uint32_t value);
   
   /* Permute each bit of 'value' from positions given in 'table'.*/
   //uint32_t permute(const uint32_t value, const uint8_t *table);
}

#endif