#include "String.hpp"

#include <algorithm>

using namespace CryptoGL;

const String String::letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const String String::uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const String String::lowercase = "abcdefghijklmnopqrstuvwxyz";
const String String::grid_uppercase_fr = "ABCDEFGHIJKLMNOPQRSTUVXYZ";
const String String::grid_lowercase_fr = "abcdefghijklmnopqrstuvxyz";
const String String::grid_uppercase_en = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
const String String::grid_lowercase_en = "abcdefghiklmnopqrstuvwxyz";
const String String::digits = "0123456789";
const String String::uppercase_digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const String String::lowercase_digits = "abcdefghijklmnopqrstuvwxyz0123456789";
const String String::hex_digits = "0123456789ABCDEF";
const String String::alpha_numeric = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const String String::ascii_ordered_alpha_numeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const String String::lower_ordered_alpha_numeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const String String::base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const String String::printable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"'().:;? \t\n\r#[]$%&*/@|_=+<>{}~";
const String String::punctuation = "!\"'().:;?-";
const String String::white_space = " \t\n\r";


void String::toUpperCase()
{
   /*for(auto c : this)
   {
      c = toupper(c);
   }*/
   std::transform(this->begin(), this->end(), this->begin(), toupper);
}

void String::toLowerCase()
{
   /*for(auto c : this)
   {
      c = tolower(c);
   }*/
   std::transform(this->begin(), this->end(), this->begin(), tolower);
}

void String::removeChars(const std::string &chars)
{
   this->erase(std::remove_if(this->begin(), this->end(), [chars] (const char c) {
      return chars.find(c) != std::string::npos;
   }), this->end());
}

void String::removeDuplicates()
{
   uint32_t str_length = this->length();
   for (uint32_t i = 0; i < str_length; ++i)
   {
      for (uint32_t j = i + 1; j < str_length; ++j)
      {
         if (this->at(i) == this->at(j))
         {
            this->erase(j, 1);
            str_length--;
         }
      }
   }
}

// Check if it exists a duplicate char in text.
bool String::containsUniqueChars() const
{
   std::vector<bool> array(256, false);

   const auto end = this->end();
   for (std::string::const_iterator it = this->begin(); it != end; ++it)
   {
      if (array[*it] == true)
      {
         return false;
      }
      else
      {
         array[*it] = true;
      }
   }

   return true;
}

std::string String::toStdString() const
{
   return std::string(this->begin(), this->end());
}

std::vector<uint8_t> String::toBytes() const
{
   return std::vector<uint8_t>(this->begin(), this->end());
}

std::vector<uint8_t> String::hexToBytes() const
{
   const uint64_t str_length = this->length();
   std::vector<uint8_t> bytes;
   bytes.reserve(str_length / 2);

   for (uint64_t i = 0; i < str_length; i += 2)
   {
      const std::string hexa = {this->at(i), this->at(i+1)};
      bytes.push_back(strtoul(hexa.c_str(), nullptr, 16));
   }

   return bytes;
}

std::string String::trimStart() const
{
   uint32_t i = 0;
   while(this->at(i) == ' ')
   {
      i++;
   }
   
   return this->substr(i);
}

std::string String::trimEnd() const
{
   uint32_t str_length = this->length() - 1;
   while(this->at(str_length) == ' ')
   {
      //this->pop_back();
      str_length--;
   }
   
   return this->substr(0, str_length + 1);
}

std::vector<String> String::split(const char separator) const
{    
    int32_t sep_pos = this->find(separator);
    if(sep_pos == -1 && !this->empty())
    {
        return std::vector<String>(1, *this);
    }
    
    std::vector<String> result;
    result.reserve(std::count(this->begin(), this->end(), separator) + 1);
    
    // find the first separator in the string if it exists and loop.
    uint32_t previous_pos = 0;
    uint32_t sub_length = sep_pos;
    while(sep_pos != -1)
    {
       result.push_back(String(this->substr(previous_pos, sub_length)));
       previous_pos = sep_pos + 1;
       sep_pos = this->find(separator, previous_pos);
       sub_length = sep_pos - previous_pos;
    }
   
    // If we didn't reach the end of str, we have to take the last substring.
    if(previous_pos < this->length())
    {
       result.push_back(String(this->substr(previous_pos)));
    }
   
    return result;
}

void String::sort()
{
   std::sort(this->begin(), this->end());
}

void String::rotateLeft(const uint64_t &to_rotate)
{
   std::rotate(this->begin(), this->begin() + to_rotate, this->end());
}

void String::rotateRight(const uint64_t &to_rotate)
{
   std::rotate(this->rbegin(), this->rbegin() + to_rotate, this->rend());
}