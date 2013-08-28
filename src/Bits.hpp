
#ifndef BITS_HPP
#define BITS_HPP

namespace Bits
{
   /* Extract the Least Signifiant Bits from value. */
   template <class UInt>
   constexpr UInt LSB(const UInt &value, const uint8_t bits_to_extract)
   {
      return value & (static_cast<UInt>(1) << bits_to_extract);
   }

   /* Extract the Most Signifiant Bits from value. */
   template <class UInt>
   constexpr UInt MSB(const UInt &value, const uint8_t bits_to_extract)
   {
      return value >> ((sizeof (UInt) << 3) - bits_to_extract);
   }

   /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,63}. */
   template <class UInt>
   constexpr UInt rotateLeft(const UInt &value, const uint8_t shift, const uint8_t max)
   {
      return ((value << shift) | (value >> (max - shift))) & ((1ull << max) - 1);
   }

   /* Rotate left 'value' of 'shift' bits of a 64-bit value. */
   constexpr uint64_t rotateLeft64(const uint64_t &value, const uint8_t shift)
   {
      return (value << shift) | (value >> (64 - shift));
   }

   /* Rotate left 'value' of 'shift' bits with 'max' in {1,...,63}. */
   template <class UInt>
   constexpr UInt rotateRight(const UInt &value, const uint8_t shift, const uint8_t max)
   {
      return ((value >> shift) | (value << (max - shift)));
   }

   /* Rotate right 'value' of 'shift' bits of a 64-bit value. */
   constexpr uint64_t rotateRight64(const uint64_t &value, const uint8_t shift)
   {
      return ((value >> shift) | (value << (64 - shift)));
   }
   
   /* Test a bit at a 'pos' in 'value'. */
   template <class UInt>
   constexpr bool getBitAtPosition(const uint8_t pos, const UInt value)
   {
      return (value & (1ull << pos)) > 0;
   }
   
   /* Set a bit at 'pos' in 'value'. */
   template <class UInt>
   constexpr UInt setBitAtPosition(const uint8_t pos, const UInt value)
   {
      return value | (1ull << pos);
   }
}

#endif