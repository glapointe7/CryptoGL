
#include "HashFunction.hpp"

/*const HashFunction::WordsContainer
HashFunction::getLittleEndianWordBlocks(const BytesContainer &bytes, const uint64_t &block_index)
{
   WordsContainer words;
   words.reserve(16);

   for (uint8_t k = 0; k < 64; k += 4)
   {
      const uint32_t word = bytes[k + block_index]
              | (bytes[k + block_index + 1] << 8)
              | (bytes[k + block_index + 2] << 16)
              | (bytes[k + block_index + 3] << 24);

      words.push_back(word);
   }

   return words;
}

const HashFunction::WordsContainer
HashFunction::getBigEndianWordBlocks(const BytesContainer &bytes, const uint64_t &block_index)
{
   WordsContainer words;
   words.reserve(16);

   for (uint8_t k = 0; k < 64; k += 4)
   {
      const uint32_t word = bytes[k + block_index + 3]
              | (bytes[k + block_index + 2] << 8)
              | (bytes[k + block_index + 1] << 16)
              | (bytes[k + block_index] << 24);
      
      words.push_back(word);
   }
   
   return words;
}

const HashFunction::BytesContainer 
HashFunction::getBigEndianOutput(const uint16_t max_words, const WordsContainer &hash)
{
   BytesContainer output;
   output.reserve(max_words << 2);

   for (uint8_t j = 0; j < max_words; ++j)
   {
      for(int8_t i = 24; i >= 0; i -= 8)
      {
         output.push_back((hash[j] >> i) & 0xFF);
      }
   }

   return output;
}

const HashFunction::BytesContainer 
HashFunction::getLittleEndianOutput(const uint16_t max_words, const WordsContainer &hash)
{
   BytesContainer output;
   output.reserve(max_words << 2);

   for (uint8_t j = 0; j < max_words; ++j)
   {
      for(uint8_t i = 0; i < 32; i += 8)
      {
         output.push_back((hash[j] >> i) & 0xFF);
      }
   }

   return output;
}*/