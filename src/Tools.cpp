
#include "Tools.hpp"

#include <algorithm>
#include <sstream>

// Check if it exists a duplicate char in text.
bool isUniqueChar(const std::string &text)
{
   std::vector<bool> array(256, 0);

   for (const auto c : text)
   {
      if (array[c] == true)
      {
         return false;
      }
      else
      {
         array[c] = true;
      }
   }

   return true;
}

const std::string removeRepeatedLetters(const std::string &str)
{
   uint32_t str_len = str.length();
   std::string result(str);

   for (uint32_t i = 0; i < str_len; ++i)
   {
      for (uint32_t j = i + 1; j < str_len; ++j)
      {
         if (result[i] == result[j])
         {
            result.erase(j, 1);
            str_len--;
         }
      }
   }

   return result;
}

void replaceChar(std::string &text, const char letter, const char rletter)
{
   std::replace_if(text.begin(), text.end(), [letter](char c) {
      return (c == letter);
   }, rletter);
}

const std::vector<std::string> split(const std::string &text)
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

const uint64_t getBitsFromTable(const uint64_t &data, const uint8_t *table, const uint8_t from, const uint8_t to)
{
   uint8_t i = to;
   uint64_t output = 0;
   
   for (uint8_t k = 0; k < to; ++k)
   {
      --i;
      if ((data >> (from - table[k])) & 0x1)
      {
         output |= 1ull << i;
      }
   }

   return output;
}

const std::vector<uint8_t> getXORedBlock(const std::vector<uint8_t> &block1, const std::vector<uint8_t> &block2)
{
   const uint64_t size = block1.size();
   std::vector<uint8_t> xored_block;
   xored_block.reserve(size);
   
   for(uint64_t i = 0; i < size; ++i)
   {
      xored_block.push_back(block1[i] ^ block2[i]);
   }
   
   return xored_block;
}