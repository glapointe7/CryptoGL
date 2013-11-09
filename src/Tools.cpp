
#include "Tools.hpp"

#include <algorithm>
#include <sstream>

const std::vector<std::string> Tools::split(const std::string &text)
{
   std::istringstream iss(text);
   std::vector<std::string> split_text;
   split_text.reserve(text.length());
   do
   {
      std::string word;
      iss >> word;
      split_text.push_back(word);
   } while (iss);
   split_text.pop_back();

   return split_text;
}

const BytesVector Tools::getXORedBlock(const BytesVector &block1, const BytesVector &block2)
{
   const uint64_t size = block1.size();
   BytesVector xored_block;
   xored_block.reserve(size);
   
   for(uint64_t i = 0; i < size; ++i)
   {
      xored_block.push_back(block1[i] ^ block2[i]);
   }
   
   return xored_block;
}

void Tools::convertMajMinToSymbol(std::string &text, const std::string symbol)
{
   std::replace_if(text.begin(), text.end(), [](char c) {
      return (isupper(c));
   }, symbol[0]);

   std::replace_if(text.begin(), text.end(), [](char c) {
      return (islower(c));
   }, symbol[1]);
}

/**
 * Convert a string from base 'from_base' to a string in base 'to_base'.
 *
 * @param string number : The number to convert.
 * @param int from_base : Numeric base of number before the conversion.
 * @param int to_base : The base for which the number will be converted.
 * @return string result
 */
const std::string Tools::baseXtoBaseY(const std::string &number, const uint8_t from_base, const uint8_t to_base)
{
   const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_";
   std::string result;
   
   BytesVector pos;
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