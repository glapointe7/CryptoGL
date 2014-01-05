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

std::vector<std::string> Vector::split(const std::string &str, const int8_t separator)
{
   std::vector<std::string> result;
   result.reserve(std::count(str.begin(), str.end(), separator) + 1);
   
   // find the first separator in the string if it exists.
   int32_t sep_pos = str.find(separator);
   uint32_t start_pos = 0;
   uint32_t sub_length = sep_pos;
   while(sep_pos != -1)
   {
      result.push_back(str.substr(start_pos, sub_length));
      start_pos = sep_pos + 1;
      sep_pos = str.find(separator, start_pos);
      sub_length = sep_pos - start_pos;
   }
   
   // If we didn't reach the end of str, we have to take the last substring.
   if(start_pos < str.length())
   {
      result.push_back(str.substr(start_pos));
   }
   
   return result;
}

std::string Vector::toString(const BytesVector &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}