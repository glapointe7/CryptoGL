
#include "BlockCipher.hpp"

const BlockCipher::BytesContainer
BlockCipher::processEncoding(const BytesContainer &data)
{
   // Append padding to get a multiple of 'input_block_length'.
   const BytesContainer data_padded = appendPadding(data, 0);
   
   const uint32_t data_len = data_padded.size();
   BytesContainer toReturn;
   toReturn.reserve(data_len);
   
   generateSubkeys();
   
   // Assuming padding is done, data_len is a multiple of 8 bytes.
   for (uint32_t n = 0; n < data_len; n += input_block_length)
   {
      const BytesContainer data_block(data_padded.begin() + n, data_padded.begin() + n + input_block_length);
      
      BytesContainer block;
      block.reserve(input_block_length);
      block = block_strategy->getCipherBlock(data_block, n >> 3);
      
      const BytesContainer output = getOutputBlock(block, true);
      toReturn.insert(toReturn.end(), output.begin(), output.end());
   }
   
   return toReturn;
}

const BlockCipher::BytesContainer
BlockCipher::processDecoding(const BytesContainer &data)
{
   const uint32_t data_len = data.size();
   BytesContainer toReturn;
   toReturn.reserve(data_len);
   
   generateInverseSubkeys();
   
   // Assuming padding is done, data_len is a multiple of 8 bytes.
   for (uint32_t n = 0; n < data_len; n += input_block_length)
   {
      const BytesContainer data_block(data.begin() + n, data.begin() + n + input_block_length);
      
      BytesContainer block;
      block.reserve(input_block_length);
      block = block_strategy->getClearBlock(data_block, n >> 3);
      
      const BytesContainer output = getOutputBlock(block, false);
      toReturn.insert(toReturn.end(), output.begin(), output.end());
   }
   
   return toReturn;
}

const BlockCipher::BytesContainer 
BlockCipher::appendPadding(const BytesContainer &data, const uint8_t fill_with) const
{
   BytesContainer full_data(data);
   const uint8_t rest = data.size() % input_block_length;
   if (rest != 0)
   {
      full_data.insert(full_data.end(), input_block_length - rest, fill_with);
   }
   
   return full_data;
}

void BlockCipher::generateInverseSubkeys()
{
   generateSubkeys();
}