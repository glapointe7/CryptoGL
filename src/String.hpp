
#ifndef STRING_HPP
#define STRING_HPP

#include <string>

class String
{
public:
    // Useful constants for alpha string.
   static const std::string letters;
   static const std::string uppercase;
   static const std::string lowercase;
   static const std::string grid_uppercase_fr;
   static const std::string grid_lowercase_fr;
   static const std::string grid_uppercase_en;
   static const std::string grid_lowercase_en;
   static const std::string digits;
   static const std::string uppercase_digits;
   static const std::string lowercase_digits;
   static const std::string hex_digits;
   static const std::string alpha_numeric;
   static const std::string ascii_ordered_alpha_numeric;
   static const std::string lower_ordered_alpha_numeric;
   static const std::string base64_alphabet;
   static const std::string printable;
   static const std::string punctuation;
   static const std::string white_space;
   
   static void upperCase(std::string &text);
   static void lowerCase(std::string &text);
   static void removeChars(std::string &text, const std::string &chars);
   static void strReverse(char* begin, char* end);
   static uint32_t strLength(const char *str); 
   static void uintToString(uint32_t value, char* str, const uint8_t base = 10);
};

#endif