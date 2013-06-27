
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>

typedef std::vector<std::vector<unsigned char> > Table;

// Prototypes pour Tools.cpp.
const std::string removeRepeatedLetters(const std::string &str);
void replaceChar(std::string &text, const char letter, const char rletter);
const std::vector<std::string> split(const std::string &text);
const std::vector<std::vector<bool> > getBlockBinary(const std::vector<bool>&, const uint32_t block_size);
const std::vector<uint8_t> getBytes(const std::vector<bool> &bits);
void eraseChars(std::string &text, const std::string chars);
bool isUniqueChar(const std::string &);

// Manipulation des bits
const bool getBitAtPosition(const unsigned long pos, const uint64_t &number);
const uint64_t rotateLeft(const uint64_t &value, const uint8_t shift, const uint8_t max);
const uint64_t rotateRight(const uint64_t &value, const uint8_t shift, const uint8_t max);
const uint64_t getBitsFromTable(const uint64_t &data, const Table &table, const uint64_t from, const uint64_t to);
const std::vector<uint8_t> getXORedBlock(const std::vector<uint8_t> &block1, const std::vector<uint8_t> &block2);

#endif
