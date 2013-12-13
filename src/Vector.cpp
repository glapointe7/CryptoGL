#include "Vector.hpp"

#include <algorithm>

BytesVector Vector::leftShift(const BytesVector &v, uint32_t shift)
{
   const uint32_t start = std::min(shift / 8, static_cast<uint32_t>(v.size()));
   BytesVector result;
   result.reserve(v.size());
   result.insert(result.end(), v.begin() + start, v.end());

   for(uint32_t i = 0; i < start; ++i) { result.push_back(0); }

   if(shift % 8 == 0 || shift >= 8 * v.size()) { return result; }

   shift %= 8;
   std::transform(result.rbegin(), result.rend(), result.rbegin(), LambdaLeftShift(shift));

   return result;
}

BytesVector Vector::rightShift(const BytesVector &v, uint32_t shift)
{
   const uint32_t start = std::min(shift / 8, static_cast<uint32_t>(v.size()));
   BytesVector result;
   result.reserve(v.size());

   for (uint32_t i = 0; i < start; ++i) { result.push_back(0); }
   result.insert(result.end(), v.begin(), v.end() - start);

   if (shift % 8 == 0 || shift >= 8 * v.size()) { return result; }

   shift %= 8;
   std::transform(result.begin(), result.end(), result.begin(), LambdaRightShift(shift));

   return result;
}