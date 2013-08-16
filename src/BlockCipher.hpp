#ifndef BLOCKCIPHER_HPP
#define BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "BlockCipherOperationModes.hpp"
#include "BlockCipherStrategy.hpp"

template <class SubkeyType, class DataType>
class BlockCipher : public SymmetricCipher
{     
public:
   /* Set an IV for all modes except ECB and CTR. */
   /*void setIV(const BytesContainer &IV)
   {
      block_strategy->setIV(IV);
   }*/
   
   /* Process general encoding for block ciphers. */
   const BytesContainer encode(const BytesContainer &message)
   {
      const BytesContainer message_padded = appendPadding(message, 0);

      const uint64_t message_padded_len = message_padded.size();
      BytesContainer output;
      output.reserve(message_padded_len);

      generateSubkeys();

      for (uint64_t n = 0; n < message_padded_len; n += input_block_length)
      {
         const BytesContainer input_block(message_padded.begin() + n, message_padded.begin() + n + input_block_length);
         
         const BytesContainer cipher_block = block_strategy->getCipherBlock(input_block);
         
         DataType int_block = getIntegersFromInputBlock(cipher_block);
         int_block = encodeBlock(int_block);
         const BytesContainer encoded_block = getOutputBlock(int_block);
         
         output.insert(output.end(), encoded_block.begin(), encoded_block.end());
      }

      return output;
   }
   
   /* Process general decoding for block ciphers. */
   const BytesContainer decode(const BytesContainer &message)
   {
      const uint64_t message_len = message.size();
      BytesContainer output;
      output.reserve(message_len);

      generateInverseSubkeys();

      for (uint64_t n = 0; n < message_len; n += input_block_length)
      {
         const BytesContainer input_block(message.begin() + n, message.begin() + n + input_block_length);
         
         const BytesContainer clear_block = block_strategy->getClearBlock(input_block);
         
         DataType int_block = getIntegersFromInputBlock(clear_block);
         int_block = decodeBlock(int_block);
         const BytesContainer decoded_block = getOutputBlock(int_block);
         
         output.insert(output.end(), decoded_block.begin(), decoded_block.end());
      }

      return output;
   }
   
protected:
   typedef std::vector<SubkeyType> SubkeysContainer;
   
   BlockCipher(const OperationModes mode, const uint8_t block_length)
      : block_strategy(BlockCipherStrategyFactory::createBlockCipherStrategy(mode)),
        input_block_length(block_length) {}
   
   virtual ~BlockCipher() { delete block_strategy; }
   
   /* Check the key provided by the user and set it if correct. */
   virtual void setKey(const BytesContainer &) = 0;
   
   /* Generate sub-keys from the key provided by the user when encoding. */
   virtual void generateSubkeys() = 0;
   
   /* Extract a vector of integers from the block of bytes. */
   virtual const DataType getIntegersFromInputBlock(const BytesContainer &block) const = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual const DataType encodeBlock(const DataType &input) = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual const DataType decodeBlock(const DataType &input) = 0;
   
   /* Extract the bytes from the vector of integers and return the encoded / decoded block. */
   virtual const BytesContainer getOutputBlock(const DataType &int_block) = 0;
   
   /* Generate sub-keys from the key provided by the user when decoding. */
   virtual void generateInverseSubkeys()
   {
      generateSubkeys();
   }
   
   /* Pad 'data' with 'input_block_length' values given by 'fill_with'. */
   const BytesContainer appendPadding(const BytesContainer &input, const uint8_t fill_with) const
   {
      BytesContainer padded_input(input);
      const uint8_t rest = input.size() % input_block_length;
      if (rest != 0)
      {
         padded_input.insert(padded_input.end(), input_block_length - rest, fill_with);
      }

      return padded_input;
   }

   BlockCipherStrategy *block_strategy;
   const uint8_t input_block_length;
   SubkeysContainer subkeys;
};

#endif