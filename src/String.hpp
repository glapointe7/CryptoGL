
#ifndef STRING_HPP
#define STRING_HPP

#include "Types.hpp"

class String1
{
public:
   explicit String1(const uint64_t &to_reserve) { str.reserve(to_reserve); }
   
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
   
   /* Uppercase the text. */
   void toUpperCase();

   /* Lowercase the text. */
   void toLowerCase();

   /* Remove all characters in text specified by chars. */
   void removeChars(const std::string &chars);
   
   /* Remove all chars that are not uniques in 'str'. */
   std::string makeUniqueChars();
   
   /* Check if 'str' contains unique chars. */
   bool containsUniqueChars() const;

   /* Convert integer to string in base 10. */
   //std::string uintToString(uint64_t value);
   
   /* Convert a string to a vector of bytes. */
   BytesVector toBytes() const;
   
   /* Convert a hexadecimal string to a vector of bytes. */
   BytesVector hexToBytes() const;
   
   /* Trim the beginning of the string from spaces. */
   std::string trimStart();
   
   /* Trim end of the string from spaces. */
   std::string trimEnd();
   
   /* Append the character 'c' 'x' times at the end of 'str'. */
   std::string extend(const uint32_t x, const char c);
   
   /* Split the string with a specific separator and return a vector of substrings. 
    Example: "AB-CD-EF".split('-') = ["AB", "CD", "EF"] */
   std::vector<std::string> split(const char separator) const;
   
   /* Find a character 'c' in the string and return the position if found, -1 if not. */
   int64_t find(const char c) const;
   
private:
   std::string str;
};


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
   std::string makeUniqueChars(std::string str);
   
   /* Check if 'str' contains unique chars. */
   bool containsUniqueChars(const std::string &str);

   /* Convert integer to string in base 10. */
   std::string uintToString(uint64_t value);
   
   /* Convert a string to a vector of bytes. */
   BytesVector toBytes(const std::string &str);
   
   /* Convert a hexadecimal string to a vector of bytes. */
   BytesVector hexToBytes(const std::string &hex_str);
   
   /* Trim the beginning of the string from spaces. */
   std::string trimStart(std::string str);
   
   /* Trim end of the string from spaces. */
   std::string trimEnd(std::string str);
   
   /* Append the character 'c' 'x' times at the end of 'str'. */
   std::string extend(std::string str, const uint32_t x, const char c);
}

#endif