#include "String.hpp"

#include <algorithm>
#include <vector>

const std::string String1::letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::string String1::uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string String1::lowercase = "abcdefghijklmnopqrstuvwxyz";
const std::string String1::grid_uppercase_fr = "ABCDEFGHIJKLMNOPQRSTUVXYZ";
const std::string String1::grid_lowercase_fr = "abcdefghijklmnopqrstuvxyz";
const std::string String1::grid_uppercase_en = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
const std::string String1::grid_lowercase_en = "abcdefghiklmnopqrstuvwxyz";
const std::string String1::digits = "0123456789";
const std::string String1::uppercase_digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const std::string String1::lowercase_digits = "abcdefghijklmnopqrstuvwxyz0123456789";
const std::string String1::hex_digits = "0123456789ABCDEF";
const std::string String1::alpha_numeric = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const std::string String1::ascii_ordered_alpha_numeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::string String1::lower_ordered_alpha_numeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const std::string String1::base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const std::string String1::printable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"'().:;? \t\n\r#[]$%&*/@|_=+<>{}~";
const std::string String1::punctuation = "!\"'().:;?-";
const std::string String1::white_space = " \t\n\r";

//void String1::toUpperCase()
//{
//   std::transform(str.begin(), str.end(), str.begin(), toupper);
//}
//
//void String1::toLowerCase()
//{
//   std::transform(str.begin(), str.end(), str.begin(), tolower);
//}
//
//void String1::removeChars(const std::string &chars)
//{
//   str.erase(std::remove_if(str.begin(), str.end(), [chars] (const char c) {
//      return chars.find(c) != std::string::npos;
//   }), str.end());
//}
//
//std::string String1::makeUniqueChars()
//{
//   uint32_t str_length = str.length();
//   for (uint32_t i = 0; i < str_length; ++i)
//   {
//      for (uint32_t j = i + 1; j < str_length; ++j)
//      {
//         if (str[i] == str[j])
//         {
//            str.erase(j, 1);
//            str_length--;
//         }
//      }
//   }
//
//   return str;
//}
//
//// Check if it exists a duplicate char in text.
//bool String1::containsUniqueChars()
//{
//   std::vector<bool> array(256, 0);
//
//   for (const auto c : str)
//   {
//      if (array[c] == true)
//      {
//         return false;
//      }
//      else
//      {
//         array[c] = true;
//      }
//   }
//
//   return true;
//}

//std::string String1::uintToString(uint64_t value)
//{
//   std::string str;
//   str.reserve(20);
//   do
//   {
//      str.push_back(static_cast<int8_t>(48 + (value % 10)));
//   }while(value /= 10);
//   std::reverse(str.begin(), str.end());
//   
//   return str;
//}

//BytesVector String1::toBytes()
//{
//   return BytesVector(str.begin(), str.end());
//}
//
//BytesVector String1::hexToBytes()
//{
//   const uint64_t str_length = str.length();
//   BytesVector bytes;
//   bytes.reserve(str_length / 2);
//
//   for (uint64_t i = 0; i < str_length; i += 2)
//   {
//      const std::string hexa = {str[i], str[i+1]};
//      bytes.push_back(strtoul(hexa.c_str(), nullptr, 16));
//   }
//
//   return bytes;
//}
//
//std::string String1::trimStart()
//{
//   uint32_t i = 0;
//   while(str[i] == ' ')
//   {
//      i++;
//   }
//   
//   return str.substr(i);
//}
//
//std::string String1::trimEnd()
//{
//   uint32_t str_length = str.length() - 1;
//   while(str[str_length] == ' ')
//   {
//      str.pop_back();
//      str_length--;
//   }
//   
//   return str;
//}
//
//std::string String1::extend(const uint32_t x, const char c)
//{
//   str.reserve(str.length() + x);
//   return str.append(std::string(x, c));
//}
//
//std::vector<std::string> String1::split(const char separator)
//{
//   std::vector<std::string> result;
//   result.reserve(std::count(str.begin(), str.end(), separator) + 1);
//   
//   // find the first separator in the string if it exists.
//   int32_t sep_pos = str.find(separator);
//   uint32_t start_pos = 0;
//   uint32_t sub_length = sep_pos;
//   while(sep_pos != -1)
//   {
//      result.push_back(str.substr(start_pos, sub_length));
//      start_pos = sep_pos + 1;
//      sep_pos = str.find(separator, start_pos);
//      sub_length = sep_pos - start_pos;
//   }
//   
//   // If we didn't reach the end of str, we have to take the last substring.
//   if(start_pos < str.length())
//   {
//      result.push_back(str.substr(start_pos));
//   }
//   
//   return result;
//}
//
//int64_t String1::find(const char c)
//{
//   return str.find(c);
//}








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