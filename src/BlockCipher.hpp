#ifndef BLOCKCIPHER_HPP
#define BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "BlockCipherOperationModes.hpp"
#include "BlockCipherModes.hpp"
#include "Endianness.hpp"

#include <functional>

template <class BytesContainer, class DataType, uint8_t InputBlockSize, class EndianType>
class InputOutputBlockGetter
{
public:
   static const BytesContainer outputBlock(const DataType &int_block)
   {
      BytesContainer output_block;
      output_block.reserve(InputBlockSize);
      const uint8_t nbSubBlocks = InputBlockSize / sizeof(typename DataType::value_type);

      EndianType endian;
      for(uint8_t i = 0; i < nbSubBlocks; ++i)
      {
         endian.toBytes(int_block[i]);
         const BytesContainer out = endian.getBytes();
         output_block.insert(output_block.end(), out.begin(), out.end());
      }

      return output_block;
   }
   
   static const DataType inputBlock(const BytesContainer &block)
   {
      DataType int_block;
      const uint8_t value_size = sizeof(typename DataType::value_type);
      const uint8_t nbSubBlocks = InputBlockSize / value_size;
      int_block.reserve(nbSubBlocks);
      
      EndianType endian;
      for(uint8_t i = 0; i < InputBlockSize; i += value_size)
      {
         endian.toInteger(BytesContainer(block.begin() + i, block.begin() + i + value_size));
         int_block.push_back(endian.getValue());
         endian.resetValue();
      }

      return int_block;
   }
};

template <class BytesContainer, class EndianType>
class InputOutputBlockGetter<BytesContainer, uint64_t, 8, EndianType>
{
public:
   static const BytesContainer outputBlock(const uint64_t &int_block)
   {
      EndianType endian;
      endian.toBytes(int_block);

      return endian.getBytes();
   }
   
   static const uint64_t inputBlock(const BytesContainer &block)
   {
      EndianType endian;
      endian.toInteger(block);

      return endian.getValue();
   }
};

template <class SubkeyType, class DataType, uint8_t InputBlockSize, class EndianType>
class BlockCipher : public SymmetricCipher
{   
public:   
   /* Process general encoding for block ciphers. */
   const BytesContainer encode(const BytesContainer &message)
   {
      const BytesContainer message_padded = appendPadding(message, 0);

      const uint64_t message_padded_len = message_padded.size();
      BytesContainer output;
      output.reserve(message_padded_len);

      generateSubkeys();

      for (uint64_t n = 0; n < message_padded_len; n += InputBlockSize)
      {
         const BytesContainer input_block(message_padded.begin() + n, message_padded.begin() + n + InputBlockSize);
         const BytesContainer encoded_block = block_mode->getCipherBlock(input_block);
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

      for (uint64_t n = 0; n < message_len; n += InputBlockSize)
      {
         const BytesContainer input_block(message.begin() + n, message.begin() + n + InputBlockSize);
         const BytesContainer decoded_block = block_mode->getClearBlock(input_block);
         output.insert(output.end(), decoded_block.begin(), decoded_block.end());
      }

      return output;
   }
   
protected:
   typedef std::vector<SubkeyType> SubkeysContainer;
   typedef BlockCipher<SubkeyType, DataType, InputBlockSize, EndianType> THIS;
   
   /* Default constructor : Only for ECB, CBC, CFB and OFB modes. An IV is needed for 
    * CBC, CFB and OFB modes. For the ECB mode, IV is empty. */
   BlockCipher(const OperationModes mode, const uint8_t round, const BytesContainer &IV)
      : block_mode(
        BlockCipherModesFactory<BytesContainer>::createBlockCipherMode(
          mode,
          IV,
          std::bind(&THIS::processEncodeBlock, this, std::placeholders::_1),
          std::bind(&THIS::processDecodeBlock, this, std::placeholders::_1))),
        rounds(round) {}
   
   /* Constructor for the CTR mode : Take a vector of IVs. */
   BlockCipher(const uint8_t round, const IVContainer &IV)
      : block_mode(
        BlockCipherModesFactory<IVContainer>::createBlockCipherMode(
          IV,
          std::bind(&THIS::processEncodeBlock, this, std::placeholders::_1))),
        rounds(round) {}
   
   virtual ~BlockCipher() { delete block_mode; }
   
   /* Check the key provided by the user and set it if correct. */
   virtual void setKey(const BytesContainer &) = 0;
   
   /* Generate sub-keys from the key provided by the user when encoding. */
   virtual void generateSubkeys() = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual const DataType encodeBlock(const DataType &input) = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual const DataType decodeBlock(const DataType &input) = 0;
   
   /* Extract a vector of integers from the block of bytes. */
   const DataType getIntegersFromInputBlock(const BytesContainer &block) const
   {
      return InputOutputBlockGetter<BytesContainer, DataType, InputBlockSize, EndianType>::inputBlock(block);
   }
   
   /* Extract the bytes from the vector of integers and return the encoded / decoded block. */
   const BytesContainer getOutputBlock(const DataType &int_block)
   {      
      return InputOutputBlockGetter<BytesContainer, DataType, InputBlockSize, EndianType>::outputBlock(int_block);
   }
   
   /* Generate sub-keys from the key provided by the user when decoding. */
   virtual void generateInverseSubkeys()
   {
      generateSubkeys();
   }
   
   /* Pad 'data' with 'input_block_length' values given by 'fill_with'. */
   const BytesContainer appendPadding(const BytesContainer &input, const uint8_t fill_with) const
   {
      BytesContainer padded_input(input);
      const uint8_t rest = input.size() % InputBlockSize;
      if (rest != 0)
      {
         padded_input.insert(padded_input.end(), InputBlockSize - rest, fill_with);
      }

      return padded_input;
   }

   BlockCipherModes *block_mode;
   uint8_t rounds;
   SubkeysContainer subkeys;
   
private:
   /* Encode an input block of bytes and return the output block. */
   const BytesContainer processEncodeBlock(const BytesContainer &block)
   {
      DataType int_block = getIntegersFromInputBlock(block);
      int_block = encodeBlock(int_block);
      
      return getOutputBlock(int_block);
   }
   
   /* Decode an input block of bytes and return the output block. */
   const BytesContainer processDecodeBlock(const BytesContainer &block)
   {
      DataType int_block = getIntegersFromInputBlock(block);
      int_block = decodeBlock(int_block);
      
      return getOutputBlock(int_block);
   }
};

#endif