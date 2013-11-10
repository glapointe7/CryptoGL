#include "String.hpp"

#include <algorithm>
#include <vector>

void String::toUpperCase(std::string &text)
{
   std::transform(text.begin(), text.end(), text.begin(), toupper);
}

void String::toLowerCase(std::string &text)
{
   std::transform(text.begin(), text.end(), text.begin(), tolower);
}

void String::removeChars(std::string &text, const std::string &chars)
{
   text.erase(std::remove_if(text.begin(), text.end(), [chars] (const char c) {
      return chars.find(c) != std::string::npos;
   }), text.end());
}

std::string String::makeUniqueChars(const std::string &str)
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

// Check if it exists a duplicate char in text.
bool String::containsUniqueChars(const std::string &str)
{
   std::vector<bool> array(256, 0);

   for (const auto c : str)
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

std::string String::uintToString(uint64_t value)
{
   std::string str;
   str.reserve(20);
   do
   {
      str.push_back(static_cast<int8_t>(48 + (value % 10)));
   }while(value /= 10);
   std::reverse(str.begin(), str.end());
   
   return str;
}