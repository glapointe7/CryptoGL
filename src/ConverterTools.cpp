#include "ConverterTools.hpp"

#include <bitset>
#include <algorithm>
#include <vector>

void convertMajMinToSymbol(std::string &text, const std::string symbol)
{
   std::replace_if(text.begin(), text.end(), [](char c) {
      return (isupper(c));
   }, symbol[0]);

   std::replace_if(text.begin(), text.end(), [](char c) {
      return (islower(c));
   }, symbol[1]);
}

std::string convertBytesToBinaryString(const std::vector<uint8_t> &bytes)
{
   std::string bits;
   bits.reserve(bytes.size() << 3);

   for (auto c : bytes)
   {
      const std::bitset<8> byte(c);
      bits.append(byte.to_string());
   }

   return bits;
}

const std::vector<bool> convertBytesToBinary(const std::vector<uint8_t> &bytes)
{
   std::vector<bool> all_bits;
   all_bits.reserve(bytes.size() << 3);
   
   for (const auto c : bytes)
   {
      const std::bitset<8> byte(c);
      for(char i = 7; i >= 0; --i)
      {
         all_bits.push_back(byte.test(i));
      }
   }

   return all_bits;
}

/**
 * Convert a string from base 'from_base' to a string in base 'to_base'.
 *
 * @param string number : The number to convert.
 * @param int from_base : Numeric base of number before the conversion.
 * @param int to_base : The base for which the number will be converted.
 * @return string result
 */
const std::string baseConvertString(const std::string &number, const uint8_t from_base,
        const uint8_t to_base)
{
   const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
   std::string result;
   
   std::vector<uint8_t> pos;
   pos.reserve(number.length());
   for (const auto c : number)
   {
      pos.push_back(chars.find(c));
   }

   uint32_t length = number.length();
   uint32_t newlen = 0;
   do
   {
      uint64_t divide = 0;
      for (uint32_t i = 0; i < length; i++)
      { // Perform division manually (which is why this works with big numbers)
         divide = divide * from_base + pos[i];
         if (divide >= to_base)
         {
            pos[newlen++] = static_cast<uint32_t> (divide / to_base);
            divide %= to_base;
         }
         else if (newlen > 0)
         {
            pos[newlen++] = 0;
         }
      }
      length = newlen;
      result.push_back(chars[divide]);
   } while (newlen != 0);

   return result;
}