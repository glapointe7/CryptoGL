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

std::string String::makeUniqueChars(std::string str)
{
   uint32_t str_len = str.length();
   for (uint32_t i = 0; i < str_len; ++i)
   {
      for (uint32_t j = i + 1; j < str_len; ++j)
      {
         if (str[i] == str[j])
         {
            str.erase(j, 1);
            str_len--;
         }
      }
   }

   return str;
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

BytesVector String::toBytes(const std::string &str)
{
   return BytesVector(str.begin(), str.end());
}

BytesVector String::hexToBytes(const std::string &hex_str)
{
   const uint64_t hex_len = hex_str.length();
   BytesVector bytes;
   bytes.reserve(hex_len / 2);

   for (uint64_t i = 0; i < hex_len; i += 2)
   {
      const std::string hexa = {hex_str[i], hex_str[i+1]};
      bytes.push_back(strtoul(hexa.c_str(), nullptr, 16));
   }

   return bytes;
}

std::string String::trimStart(std::string str)
{
   uint32_t i = 0;
   while(str[i] == ' ')
   {
      i++;
   }
   
   return str.substr(i);
}

std::string String::trimEnd(std::string str)
{
   uint32_t str_length = str.length() - 1;
   while(str[str_length] == ' ')
   {
      str.pop_back();
      str_length--;
   }
   
   return str;
}

std::string String::extend(std::string str, const uint32_t x, const char c)
{
   str.reserve(str.length() + x);
   return str.append(std::string(x, c));
}