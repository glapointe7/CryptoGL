#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <vector>

namespace CryptoGL
{
    /* Decorate from std::string : Add features and useful constants. */
    class String : public std::string
    {
    public:
        /* Default constructor. */
        String() : std::string("") { }
        
        /* Constructor that reserve 'to_reserve' bytes to a string. */
        explicit String(const uint64_t &to_reserve)
        {
            this->reserve(to_reserve);
        }
        
        /* Constructor that converts char* to a std::string. */
        String(const char* str) : std::string(str) { }
        
        /* Constructor that converts String to std::string. */
        explicit String(const std::string &str) : std::string(str) { }
        
        /* Constructor that fill a string with character 'c' 'n' times. */
        String(const uint64_t &n, const uint8_t c) : std::string(n, c) { }
        
        /* Move constructor. */
        String(std::string &&str) : std::string(str) { }
        
        /* Copies the sequence of characters in the range [first,last), in the same order. */
        template <class InputIterator>
        String(InputIterator first, InputIterator last) : std::string(first, last) { }
        
        /* Like the copy constructor. */
        String operator = (const std::string &str) 
        { 
            return String(str); 
        }

        // Useful constants for alpha string.
        static const String LETTERS;
        static const String UPPERCASE;
        static const String LOWERCASE;
        static const String GRID_UPPERCASE_FR;
        static const String GRID_LOWERCASE_FR;
        static const String GRID_UPPERCASE_EN;
        static const String GRID_LOWERCASE_EN;
        static const String DIGITS;
        static const String UPPERCASE_DIGITS;
        static const String LOWERCASE_DIGITS;
        static const String HEX_DIGITS;
        static const String ALPHA_NUMERIC;
        static const String ASCII_ORDERED_ALPHA_NUMERIC;
        static const String LOWER_ORDERED_ALPHA_NUMERIC;
        static const String BASE64_ALPHABET;
        static const String PRINTABLE;
        static const String PUNCTUATION;
        static const String WHITE_SPACE;

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
}

#endif