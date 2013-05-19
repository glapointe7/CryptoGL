
#include "BlockCipher.hpp"

#include "exceptions/BadKeyLength.hpp"

const BlockCipher::BytesContainer
BlockCipher::process(const BytesContainer &data, const int8_t lower_round)
{
   const uint32_t data_len = data.size();
   BytesContainer toReturn;
   toReturn.reserve(data_len);

   for (uint32_t n = 0; n < data_len; n += 8)
   {
      // Get data block.
      const BytesContainer data_block(data.begin() + n, data.begin() + n + 8);
      
      BytesContainer block;
      block.reserve(8);
      
      // Decode if lower_round < 0.
      if (lower_round < 0)
      {
         block = block_strategy->getClearBlock(data_block, n >> 3);
      }
      else
      {
         block = block_strategy->getCipherBlock(data_block, n >> 3);
      }
      
      const BytesContainer output = getOutputBlock(block, lower_round);
      toReturn.insert(toReturn.end(), output.begin(), output.end());
   }

   return toReturn;
}