
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

// Manipulation of bits
const bool getBitAtPosition(const unsigned long pos, const uint64_t &number);
const uint64_t getBitsFromTable(const uint64_t &data, const uint8_t *table, const uint8_t from, const uint8_t to);
const std::vector<uint8_t> getXORedBlock(const std::vector<uint8_t> &block1, const std::vector<uint8_t> &block2);

template <class UInt>
UInt LSB(const UInt &value, const uint8_t bits_to_extract)
{
   return value & (1ull << bits_to_extract);
}

template <class UInt>
UInt MSB(const UInt &value, const uint8_t bits_to_extract)
{
   return value >> ((sizeof(UInt) << 3) - bits_to_extract);
}

/* Rotate left 'value' of 'shift' bits with 'max' in {1,...,63}. */
template <class UInt>
UInt rotateLeft(const UInt &value, const uint8_t shift, const uint8_t max)
{
   return ((value << shift) | (value >> (max - shift))) & ((1ull << max) - 1);
}

uint64_t rotateLeft64(const uint64_t &value, const uint8_t shift);

template <class UInt>
UInt rotateRight(const UInt &value, const uint8_t shift, const uint8_t max)
{
   return ((value >> shift) | (value << (max - shift)));
}

uint64_t rotateRight64(const uint64_t &value, const uint8_t shift);

#endif
