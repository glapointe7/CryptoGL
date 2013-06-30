#include "String.hpp"

#include <algorithm>
#include <map>

void String::toUpperCase(std::string &text)
{
   std::transform(text.begin(), text.end(), text.begin(), toupper);
}

void String::toLowerCase(std::string &text)
{
   std::transform(text.begin(), text.end(), text.begin(), tolower);
}

// Remove all characters in text specified by chars.

void String::removeChars(std::string &text, const std::string &chars)
{
   text.erase(std::remove_if(text.begin(), text.end(), [chars] (const char c) {
      return chars.find(c) != std::string::npos;
   }), text.end());
}

/* Reverse a string. */
void String::strReverse(char* begin, char* end)
{
   char aux;
   while (end > begin)
      aux = *end, *end-- = *begin, *begin++ = aux;
}

uint32_t String::strLength(const char *str)
{
   uint32_t i = 0;
   while (*str != '\0')
   {
      str++;
      ++i;
   }

   return i;
}

/* Convert integer to string in base 2 to 10. */
void String::uintToString(uint32_t value, char *str)
{
   //const uint8_t base = 10;
   char* wstr = str;
   // Conversion : Number is reversed.
   do
   {
      *wstr++ = static_cast<char> (48 + (value % 10));
   } while (value /= 10);
   *wstr = '\0';
   // Reverse string
   strReverse(str, wstr - 1);
}