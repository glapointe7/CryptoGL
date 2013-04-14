
#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>

// Prototypes pour Tools.cpp.
std::string removeRepeatedLetters(const std::string key);
void replaceChar(std::string &text, const char letter, const char rletter);
std::string getBinary(const std::string &bin_text, const std::string &bin_to_compare, const unsigned char op);
std::string getBinaryShifter(const std::string &bin_text, const unsigned int shift, bool is_right);
std::string convertBinaryToString(std::string &bin_text);
std::vector<std::string> split(const std::string &text);
std::vector<std::vector<bool> > getBlockBinary(const std::vector<bool>&, const unsigned long block_size);
std::string getStepCharacters(std::string &text, unsigned short step);
std::vector<unsigned char> getBytes(const std::vector<bool> &bits);
void eraseChars(std::string &text, const std::string chars);

// Fonctions mathématiques.
uint_fast32_t getPGCD(uint_fast32_t a, uint_fast32_t b);
long getIntegerMod(long n, long mod);
long getModInverse(long a, const long n);
bool isSuperIncresing(const std::vector<unsigned long> &sequence);

#endif