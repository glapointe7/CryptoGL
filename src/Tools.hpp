
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>

typedef std::vector<std::vector<unsigned char> > Table;

// Prototypes pour Tools.cpp.
const std::string removeRepeatedLetters(const std::string key);
void replaceChar(std::string &text, const char letter, const char rletter);
std::string getBinary(const std::string &bin_text, const std::string &bin_to_compare, const unsigned char op);
std::string getBinaryShifter(const std::string &bin_text, const unsigned int shift, bool is_right);
//std::string convertBinaryToString(std::string &bin_text);
const std::vector<std::string> split(const std::string &text);
std::vector<std::vector<bool> > getBlockBinary(const std::vector<bool>&, const unsigned long block_size);
std::string getStepCharacters(std::string &text, unsigned short step);
std::vector<unsigned char> getBytes(const std::vector<bool> &bits);
void eraseChars(std::string &text, const std::string chars);
bool isUniqueChar(const std::string &);

// Manipulation des bits
const bool getBitAtPosition(const unsigned long pos, const uint64_t &number);
uint32_t rotl32(const uint32_t number, const unsigned char pos);
uint32_t rotr32(const uint32_t number, const unsigned char pos);
uint32_t getBitsRange(const uint32_t number, const unsigned int from, const unsigned int to);
const uint64_t rotateLeft(const int64_t &value, const unsigned char shift, const unsigned char max);
const uint64_t rotateRight(const int64_t &value, const unsigned char shift, const unsigned char max);
const uint64_t getBitsFromTable(const uint64_t &data, const Table &table, const uint64_t from, const uint64_t to);
void endianSwap32(uint32_t &value);
void endianSwap64(uint64_t &value);
const std::vector<uint8_t> getXORedBlock(const std::vector<uint8_t> &block1, const std::vector<uint8_t> &block2);

#endif
