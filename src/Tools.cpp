
#include "Tools.hpp"

#include <algorithm>
#include <sstream>

// Vérifie s'il existe un doublons dans text.
bool isUniqueChar(const std::string &text)
{
   std::vector<bool> array(256, 0);

   for (const auto c : text)
   {
      if (array[c] == true)
      {
         return false;
      }
      else
      {
         array[c] = true;
      }
   }

   return true;
}

const std::string removeRepeatedLetters(const std::string &str)
{
   uint32_t str_len = str.length();
   std::string result(str);

   for (uint32_t i = 0; i < str_len; ++i)
   {
      for (uint32_t j = i + 1; j < str_len; ++j)
      {
         if (result[i] == result[j])
         {
            result.erase(j, 1);
            str_len--;
         }
      }
   }

   return result;
}

// Remplace dans "text" chaque occurence du caract�re "letter" par "rletter".

void replaceChar(std::string &text, const char letter, const char rletter)
{
   std::replace_if(text.begin(), text.end(), [letter](char c) {
      return (c == letter);
   }, rletter);
}

// S�pare une cha�ne de caract�res par l'espace et met chaque mot dans un vecteur.

const std::vector<std::string> split(const std::string &text)
{
   std::istringstream iss(text);
   std::vector<std::string> split_text;
   split_text.reserve(text.length());
   do
   {
      std::string word;
      iss >> word;
      split_text.push_back(word);
   } while (iss);
   split_text.pop_back();

   return split_text;
}

const std::vector<std::vector<bool> > getBlockBinary(const std::vector<bool> &bytes, const uint32_t block_size)
{
   const uint32_t bytes_size = bytes.size();
   std::vector<std::vector<bool> > blocks_bits;
   blocks_bits.reserve(bytes_size / block_size);

   for (uint32_t i = 0; i < bytes_size; i += block_size)
   {
      const std::vector<bool> block(bytes.begin() + i, bytes.begin() + i + block_size);
      blocks_bits.push_back(block);
   }

   return blocks_bits;
}

const std::vector<uint8_t> getBytes(const std::vector<bool> &bits)
{
   const uint32_t bits_size = bits.size();
   std::vector<uint8_t> bytes;
   bytes.reserve(bits_size >> 3);

   for (uint32_t i = 0; i < bits_size; i += 8)
   {
      uint8_t k = 0;
      uint8_t ascii = 0;
      for (char j = 7; j >= 0; --j)
      {
         ascii += bits[i + k] << j;
         k++;
      }
      bytes.push_back(ascii);
   }

   return bytes;
}

void eraseChars(std::string &text, const std::string chars)
{
   for (const auto c : chars)
   {
      text.erase(std::remove(text.begin(), text.end(), c), text.end());
   }
}

// Retourne le bit à la position pos de number.

const bool getBitAtPosition(const unsigned long pos, const uint64_t &number)
{
   return (number & (1ull << pos)) > 0;
}

const uint64_t rotateRight(const int64_t &value, const uint8_t shift, const uint8_t max)
{
   return ((value >> shift) | (value << (max - shift))) & ((1ull << max) - 1);
}

// bits doit être <= 64. 

const uint64_t rotateLeft(const int64_t &value, const uint8_t shift, const uint8_t max)
{
   return ((value << shift) | (value >> (max - shift))) & ((1ull << max) - 1);
}

// NOTE : Le premier bit correspond au MSB de data, donc 2^63 pour un 64 bits.

const uint64_t getBitsFromTable(const uint64_t &data, const Table &table, const uint64_t from, const uint64_t to)
{
   uint8_t i = to;
   uint64_t output = 0;
   for (const auto row : table)
   {
      for (const auto byte : row)
      {
         --i;
         // Si le bit à la position byte MSB est 1, alors on le positionne selon i MSB.
         if ((data >> (from - byte)) & 0x1)
         {
            output |= 1ull << i;
         }
      }
   }

   return output;
}

const std::vector<uint8_t> getXORedBlock(const std::vector<uint8_t> &block1, const std::vector<uint8_t> &block2)
{
   const uint32_t size = block1.size();
   std::vector<uint8_t> xored_block;
   xored_block.reserve(size);
   
   for(uint8_t i = 0; i < size; ++i)
   {
      xored_block.push_back(block1[i] ^ block2[i]);
   }
   
   return xored_block;
}