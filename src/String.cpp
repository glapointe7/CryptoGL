#include "String.hpp"

#include <algorithm>
#include <map>

const std::string
String::letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

const std::string
String::uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const std::string
String::lowercase = "abcdefghijklmnopqrstuvwxyz";

const std::string
String::grid_uppercase_fr = "ABCDEFGHIJKLMNOPQRSTUVXYZ";

const std::string
String::grid_lowercase_fr = "abcdefghijklmnopqrstuvxyz";

const std::string
String::grid_uppercase_en = "ABCDEFGHIKLMNOPQRSTUVWXYZ";

const std::string
String::grid_lowercase_en = "abcdefghiklmnopqrstuvwxyz";

const std::string
String::digits = "0123456789";

const std::string
String::uppercase_digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

const std::string
String::lowercase_digits = "abcdefghijklmnopqrstuvwxyz0123456789";

const std::string
String::hex_digits = "0123456789ABCDEF";

const std::string
String::alpha_numeric = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

const std::string 
String::ascii_ordered_alpha_numeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

const std::string 
String::lower_ordered_alpha_numeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

const std::string
String::base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const std::string
String::punctuation = "!\"'().:;?-";

const std::string
String::white_space = " \t\n\r";

const std::string
String::printable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"'().:;? \t\n\r#[]$%&*/@|_=+<>{}~";

// Uppercase the text.

void String::upperCase(std::string &text)
{
   std::transform(text.begin(), text.end(), text.begin(), toupper);
}

// Lowercase the text.

void String::lowerCase(std::string &text)
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