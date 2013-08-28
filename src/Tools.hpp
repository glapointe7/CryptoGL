
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <string>
#include <vector>

const std::string removeRepeatedLetters(const std::string &str);
void replaceChar(std::string &text, const char letter, const char rletter);
const std::vector<std::string> split(const std::string &text);
//const std::vector<std::vector<bool> > getBlockBinary(const std::vector<bool>&, const uint32_t block_size);
//const std::vector<uint8_t> getBytes(const std::vector<bool> &bits);
bool isUniqueChar(const std::string &);

// Manipulation of bits
const uint64_t getBitsFromTable(const uint64_t &data, const uint8_t *table, const uint8_t from, const uint8_t to);
const std::vector<uint8_t> getXORedBlock(const std::vector<uint8_t> &block1, const std::vector<uint8_t> &block2);

#endif