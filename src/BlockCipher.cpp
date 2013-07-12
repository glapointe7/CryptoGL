
#include "BlockCipher.hpp"

const BlockCipher::BytesContainer
BlockCipher::processEncode(const BytesContainer &data, const uint8_t block_len)
{
   const uint32_t data_len = data.size();
   BytesContainer toReturn;
   toReturn.reserve(data_len);
   
   generateSubkeys();
   
   // Assuming padding is done, data_len is a multiple of 8 bytes.
   for (uint32_t n = 0; n < data_len; n += block_len)
   {
      const BytesContainer data_block(data.begin() + n, data.begin() + n + block_len);
      
      BytesContainer block;
      block.reserve(block_len);
      block = block_strategy->getCipherBlock(data_block, n >> 3);
      
      const BytesContainer output = getOutputBlock(block, true);
      toReturn.insert(toReturn.end(), output.begin(), output.end());
   }
   
   return toReturn;
}

const BlockCipher::BytesContainer
BlockCipher::process(const BytesContainer &data, const uint8_t block_len, const bool to_encode)
{
   const uint32_t data_len = data.size();
   BytesContainer toReturn;
   toReturn.reserve(data_len);

   if(to_encode)
   {
      generateSubkeys();
   }
   else
   {
      generateInverseSubkeys();
   }
   
   // Assuming padding is done, data_len is a multiple of 8 bytes.
   for (uint32_t n = 0; n < data_len; n += block_len)
   {
      const BytesContainer data_block(data.begin() + n, data.begin() + n + block_len);
      
      BytesContainer block;
      block.reserve(block_len);
      
      // Decode if lower_round < 0 and apply strategy design to use the correct operation mode.
      if (!to_encode)
      {
         block = block_strategy->getClearBlock(data_block, n >> 3);
      }
      else
      {
         block = block_strategy->getCipherBlock(data_block, n >> 3);
      }
      
      // Get Encode / Decode blocks.
      const BytesContainer output = getOutputBlock(block, to_encode);
      toReturn.insert(toReturn.end(), output.begin(), output.end());
   }

   return toReturn;
}

const BlockCipher::BytesContainer 
BlockCipher::addPadding(const BytesContainer &data, const uint32_t block_length, const uint8_t fill_with)
{
   BytesContainer full_data(data);
   const uint8_t rest = data.size() % block_length;
   if (rest != 0)
   {
      full_data.insert(full_data.end(), block_length - rest, fill_with);
   }
   
   return full_data;
}

void BlockCipher::generateInverseSubkeys()
{
   generateSubkeys();
}