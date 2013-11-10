
#ifndef STRING_HPP
#define STRING_HPP

#include <string>

namespace String
{
   // Useful constants for alpha string.
   const std::string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
   const std::string grid_uppercase_fr = "ABCDEFGHIJKLMNOPQRSTUVXYZ";
   const std::string grid_lowercase_fr = "abcdefghijklmnopqrstuvxyz";
   const std::string grid_uppercase_en = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
   const std::string grid_lowercase_en = "abcdefghiklmnopqrstuvwxyz";
   const std::string digits = "0123456789";
   const std::string uppercase_digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   const std::string lowercase_digits = "abcdefghijklmnopqrstuvwxyz0123456789";
   const std::string hex_digits = "0123456789ABCDEF";
   const std::string alpha_numeric = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
   const std::string ascii_ordered_alpha_numeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   const std::string lower_ordered_alpha_numeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   const std::string base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const std::string printable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"'().:;? \t\n\r#[]$%&*/@|_=+<>{}~";
   const std::string punctuation = "!\"'().:;?-";
   const std::string white_space = " \t\n\r";

   /* Uppercase the text. */
   void toUpperCase(std::string &text);

   /* Lowercase the text. */
   void toLowerCase(std::string &text);

   /* Remove all characters in text specified by chars. */
   void removeChars(std::string &text, const std::string &chars);
   
   /* Remove all chars that are not uniques in 'str'. */
   std::string makeUniqueChars(const std::string &str);
   
   /* Check if 'str' contains unique chars. */
   bool containsUniqueChars(const std::string &str);

   /* Convert integer to string in base 10. */
   std::string uintToString(uint64_t value);
}

#endif