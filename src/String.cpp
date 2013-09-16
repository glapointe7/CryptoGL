#include "String.hpp"

#include <algorithm>

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

const std::string String::uintToString(uint64_t value)
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