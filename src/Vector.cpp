#include "Vector.hpp"

#include <algorithm>

uint8_t Vector::shr(uint8_t value, const uint8_t to_shift)
{
   return value >> to_shift;
}

uint8_t Vector::shl(uint8_t value, const uint8_t to_shift)
{
   return value << to_shift;
}

BytesVector Vector::leftShift(const BytesVector &v, uint32_t shift)
{
   const uint32_t v_size = v.size();
   const uint32_t start = std::min(shift / 8, v_size);
   BytesVector result(v.begin() + start, v.end());
   result.reserve(v_size);

   result.insert(result.end(), start, 0);

   if(shift % 8 == 0 || shift >= 8 * v_size) { return result; }

   shift %= 8;
   std::transform(result.rbegin(), result.rend(), result.rbegin(), Shift(shift, shl, shr));

   return result;
}

BytesVector Vector::rightShift(const BytesVector &v, uint32_t shift)
{
   const uint32_t v_size = v.size();
   const uint32_t start = std::min(shift / 8, v_size);
   BytesVector result(start, 0);
   result.reserve(v_size);

   extend(result, v, 0, v_size - start);

   if (shift % 8 == 0 || shift >= 8 * v_size) { return result; }

   shift %= 8;
   std::transform(result.begin(), result.end(), result.begin(), Shift(shift, shr, shl));

   return result;
}

BytesVector Vector::rotateLeft(BytesVector V, const uint32_t to_rotate)
{
   const uint8_t bytes_to_rotate = to_rotate / 8;
   const uint8_t bits_to_shift = to_rotate % 8;
   const uint8_t rest = 8 - bits_to_shift;
   const uint64_t V_size = V.size();

   BytesVector rotated_bytes;
   rotated_bytes.reserve(V_size);
   for (uint8_t i = 0; i < V_size; ++i) 
   {
      rotated_bytes.push_back((V[(i+bytes_to_rotate) % V_size] << bits_to_shift) 
                            | (V[(i+bytes_to_rotate+1) % V_size] >> rest));
   }

   return rotated_bytes;
}

BytesVector Vector::rotateRight(BytesVector V, const uint32_t to_rotate)
{
   const uint64_t V_size = V.size();
   const uint64_t V_size_bits = V_size * 8;
   const uint64_t V_size_rotate = V_size_bits - to_rotate;
   const uint64_t to_int = V_size_rotate / 8;
   const BytesVector W = leftShift(V, V_size_rotate);
   V = rightShift(V, to_rotate);
   for(uint64_t i = 0; i < to_int; ++i)
   {
      V[i] = W[i];
   }
   
   return V;
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