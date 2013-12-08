#include "Vector.hpp"

#include <algorithm>

BytesVector Vector::leftShift(const BytesVector &v, uint32_t shift)
{
   class LeftShift 
   {
      uint8_t carry;
      const uint32_t shift;
      const uint32_t inner_shift;
      const uint8_t LEFT_MASK;

   public:
      explicit LeftShift(const uint32_t shift)
         : carry(0), shift(shift), inner_shift(8 - shift), LEFT_MASK(255u << inner_shift) {}

      uint8_t operator()(uint8_t val) 
      {
         const uint8_t left = val & LEFT_MASK;
         val <<= shift;
         val |= carry;

         carry = left >> inner_shift;
         return val;
      }
   };

   const uint32_t start = std::min(shift / 8, static_cast<uint32_t>(v.size()));
   BytesVector result;
   result.reserve(v.size());
   result.insert(result.end(), v.begin() + start, v.end());

   for(uint32_t i = 0; i < start; ++i) { result.push_back(0); }

   if(shift % 8 == 0 || shift >= 8 * v.size()) { return result; }

   shift %= 8;
   std::transform(result.rbegin(), result.rend(), result.rbegin(), LeftShift(shift));

   return result;
}

BytesVector Vector::rightShift(const BytesVector &v, uint32_t shift)
{
   class RightShift 
   {
      uint8_t carry;
      const uint32_t shift;
      const uint32_t inner_shift;
      const uint8_t RIGHT_MASK;

   public:
      explicit RightShift(const uint32_t shift)
         : carry(0), shift(shift), inner_shift(8 - shift), RIGHT_MASK(0xFF >> inner_shift) {}

      uint8_t operator()(uint8_t val) 
      {
         const uint8_t right = val & RIGHT_MASK;
         val >>= shift;
         val |= carry;

         carry = right << inner_shift;
         return val;
      }
   };

   const uint32_t start = std::min(shift / 8, static_cast<uint32_t>(v.size()));
   BytesVector result;
   result.reserve(v.size());

   for (uint32_t i = 0; i < start; ++i) { result.push_back(0); }
   result.insert(result.end(), v.begin(), v.end() - start);

   if (shift % 8 == 0 || shift >= 8 * v.size()) { return result; }

   shift %= 8;
   std::transform(result.begin(), result.end(), result.begin(), RightShift(shift));

   return result;
}