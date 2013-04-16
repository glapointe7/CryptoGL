
#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>
#include <bitset>

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

// Manipulation des bits
bool getBitAtPosition(const unsigned long pos, const uint64_t number);
uint32_t setBit(const uint32_t number, const unsigned char pos);
uint32_t rotl32(const uint32_t number, const unsigned char pos);
uint32_t rotr32(const uint32_t number, const unsigned char pos);
uint32_t getBitsRange(const uint32_t number, const unsigned int from, const unsigned int to);
uint32_t rotateLeft(const int32_t number, const unsigned char n, const unsigned char bits);

template<class uint_type1, class uint_type2> 
const uint_type1 getBitsFromTable(const uint_type2 &data, const std::vector<std::vector<unsigned char> > &table)
{
   uint_type1 output = 0;
   for (auto row : table)
   {
      for (auto byte : row)
      {
         if (getBitAtPosition(byte, data) == true)
         {
            output = setBit(output, byte - 1);
         }
      }
   }

   return output;
}

// Fonctions mathématiques.
uint_fast32_t getPGCD(uint_fast32_t a, uint_fast32_t b);
long getIntegerMod(long n, long mod);
long getModInverse(long a, const long n);
bool isSuperIncresing(const std::vector<unsigned long> &sequence);
uint64_t getLegendreSymbol(uint64_t x, uint64_t e, uint64_t n);

#endif
