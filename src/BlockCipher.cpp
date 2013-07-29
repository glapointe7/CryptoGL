
#include "BlockCipher.hpp"

void BlockCipher::setIV(const BytesContainer &IV)
{
   block_strategy->setIV(IV);
}

const BlockCipher::BytesContainer
BlockCipher::processEncoding(const BytesContainer &input)
{
   const BytesContainer input_padded = appendPadding(input, 0);
   
   const uint64_t input_padded_len = input_padded.size();
   BytesContainer output;
   output.reserve(input_padded_len);
   
   generateSubkeys();
   
   // Assuming padding is done, data_len is a multiple of 'input_block_length' bytes.
   for (uint64_t n = 0; n < input_padded_len; n += input_block_length)
   {
      const BytesContainer input_block(input_padded.begin() + n, input_padded.begin() + n + input_block_length);
      
      const BytesContainer cipher_block = block_strategy->getCipherBlock(input_block, n / input_block_length);
      
      const BytesContainer encoded_block = getOutputBlock(cipher_block, true);
      output.insert(output.end(), encoded_block.begin(), encoded_block.end());
   }
   
   return output;
}

const BlockCipher::BytesContainer
BlockCipher::processDecoding(const BytesContainer &input)
{
   const uint64_t input_len = input.size();
   BytesContainer output;
   output.reserve(input_len);
   
   generateInverseSubkeys();
   
   for (uint64_t n = 0; n < input_len; n += input_block_length)
   {
      const BytesContainer input_block(input.begin() + n, input.begin() + n + input_block_length);
      
      const BytesContainer clear_block = block_strategy->getClearBlock(input_block, n / input_block_length);
      
      const BytesContainer decoded_block = getOutputBlock(clear_block, false);
      output.insert(output.end(), decoded_block.begin(), decoded_block.end());
   }
   
   return output;
}

const BlockCipher::BytesContainer 
BlockCipher::appendPadding(const BytesContainer &input, const uint8_t fill_with) const
{
   BytesContainer padded_input(input);
   const uint8_t rest = input.size() % input_block_length;
   if (rest != 0)
   {
      padded_input.insert(padded_input.end(), input_block_length - rest, fill_with);
   }
   
   return padded_input;
}

void BlockCipher::generateInverseSubkeys()
{
   generateSubkeys();
}