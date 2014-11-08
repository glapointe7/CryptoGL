
#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <vector>

/* Inherits from std::string : Add features and useful constants. */
class String : public std::string
{
public:
   /* Default constructor. */
   String() : std::string("") {}
   
   /* Constructor that reserve 'to_reserve' bytes to a string. */
   explicit String(const uint64_t &to_reserve) { this->reserve(to_reserve); }
   
   /* Constructor that converts char* to a std::string. */
   String(const char* str) : std::string(str) { }
   
   /* Constructor that converts String to std::string. */
   explicit String(const std::string &str) : std::string(str) { }
   
   /* Constructor that fill a string with character 'c' 'n' times. */
   String(const uint64_t &n, const uint8_t c) : std::string(n, c) {}
   
   /* Move constructor. */
   String(std::string &&str) : std::string(str) {}
   
   // Useful constants for alpha string.
   static const String letters;
   static const String uppercase;
   static const String lowercase;
   static const String grid_uppercase_fr;
   static const String grid_lowercase_fr;
   static const String grid_uppercase_en;
   static const String grid_lowercase_en;
   static const String digits;
   static const String uppercase_digits;
   static const String lowercase_digits;
   static const String hex_digits;
   static const String alpha_numeric;
   static const String ascii_ordered_alpha_numeric;
   static const String lower_ordered_alpha_numeric;
   static const String base64_alphabet;
   static const String printable;
   static const String punctuation;
   static const String white_space;
   
   /* Uppercase the text. */
   void toUpperCase();

   /* Lowercase the text. */
   void toLowerCase();

   /* Remove all characters in text specified by chars. */
   void removeChars(const std::string &chars);
   
   /* Remove all chars that are not uniques in 'str'. */
   void removeDuplicates();
   
   /* Check if 'str' contains unique chars. */
   bool containsUniqueChars() const;
   
   /* Cast from String to std::string. */
   std::string toStdString() const;
   
   /* Convert a string to a vector of bytes. */
   std::vector<uint8_t> toBytes() const;
   
   /* Convert a hexadecimal string to a vector of bytes. */
   std::vector<uint8_t> hexToBytes() const;
   
   /* Trim the beginning of the string from spaces. */
   std::string trimStart() const;
   
   /* Trim end of the string from spaces. */
   std::string trimEnd() const;
   
   /* Split the string with a specific separator and return a vector of substrings. 
    Example: "AB-CD-EF".split('-') = ["AB", "CD", "EF"] */
   std::vector<String> split(const char separator) const;
   
   /* Sort the entire string. */
   void sort();
   
   /* Rotate left of 'to_rotate' characters in string. */
   void rotateLeft(const uint64_t &to_rotate);
   
   /* Rotate right of 'to_rotate' characters in string. */
   void rotateRight(const uint64_t &to_rotate);
};

#endif