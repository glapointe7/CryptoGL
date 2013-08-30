#ifndef BLOCKCIPHER_HPP
#define BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "BlockCipherOperationModes.hpp"
#include "BlockCipherModes.hpp"
#include "BigEndian.hpp"
#include "LittleEndian.hpp"

#include <functional>

template <class BytesVector, class DataType, uint8_t InputBlockSize, class EndianType>
class InputOutputBlockGetter
{
public:
   static const BytesVector outputBlock(const DataType &int_block)
   {
      BytesVector output_block;
      output_block.reserve(InputBlockSize);
      const uint8_t nbSubBlocks = InputBlockSize / sizeof(typename DataType::value_type);

      for(uint8_t i = 0; i < nbSubBlocks; ++i)
      {
         const BytesVector out = EndianType::toBytesVector(int_block[i]);
         output_block.insert(output_block.end(), out.begin(), out.end());
      }

      return output_block;
   }
   
   static const DataType inputBlock(const BytesVector &block)
   {
      DataType int_block;
      const uint8_t value_size = sizeof(typename DataType::value_type);
      const uint8_t nbSubBlocks = InputBlockSize / value_size;
      int_block.reserve(nbSubBlocks);
      
      for(uint8_t i = 0; i < InputBlockSize; i += value_size)
      {
         int_block.push_back(EndianType::toInteger(BytesVector(block.begin() + i, block.begin() + i + value_size)));
      }

      return int_block;
   }
};

template <class BytesVector, class EndianType>
class InputOutputBlockGetter<BytesVector, uint64_t, 8, EndianType>
{
public:
   static const BytesVector outputBlock(const uint64_t &int_block)
   {
      return EndianType::toBytesVector(int_block);
   }
   
   static const uint64_t inputBlock(const BytesVector &block)
   {
      return EndianType::toInteger(block);
   }
};

template <class SubkeyType, class DataType, uint8_t InputBlockSize, class EndianType>
class BlockCipher : public SymmetricCipher
{   
public:   
   /* Process general encoding for block ciphers. */
   const BytesVector encode(const BytesVector &message)
   {
      const BytesVector message_padded = appendPadding(message, 0);

      const uint64_t message_padded_len = message_padded.size();
      BytesVector output;
      output.reserve(message_padded_len);

      generateSubkeys();

      for (uint64_t n = 0; n < message_padded_len; n += InputBlockSize)
      {
         const BytesVector input_block(message_padded.begin() + n, message_padded.begin() + n + InputBlockSize);
         const BytesVector encoded_block = block_mode->getCipherBlock(input_block);
         output.insert(output.end(), encoded_block.begin(), encoded_block.end());
      }

      return output;
   }
   
   /* Process general decoding for block ciphers. */
   const BytesVector decode(const BytesVector &message)
   {
      const uint64_t message_len = message.size();
      BytesVector output;
      output.reserve(message_len);

      generateInverseSubkeys();

      for (uint64_t n = 0; n < message_len; n += InputBlockSize)
      {
         const BytesVector input_block(message.begin() + n, message.begin() + n + InputBlockSize);
         const BytesVector decoded_block = block_mode->getClearBlock(input_block);
         output.insert(output.end(), decoded_block.begin(), decoded_block.end());
      }

      return output;
   }
   
protected:
   typedef std::vector<SubkeyType> SubkeysContainer;
   typedef BlockCipher<SubkeyType, DataType, InputBlockSize, EndianType> THIS;
   
   /* Default constructor : Only for ECB, CBC, CFB and OFB modes. An IV is needed for 
    * CBC, CFB and OFB modes. For the ECB mode, IV is empty. */
   BlockCipher(const OperationModes mode, const uint8_t round, const BytesVector &IV)
      : block_mode(
        BlockCipherModesFactory<BytesVector>::createBlockCipherMode(
          mode,
          IV,
          std::bind(&THIS::processEncodeBlock, this, std::placeholders::_1),
          std::bind(&THIS::processDecodeBlock, this, std::placeholders::_1))),
        rounds(round) {}
   
   /* Constructor for the CTR mode : Take a vector of IVs. */
   BlockCipher(const uint8_t round, const IV_Vector &IV)
      : block_mode(
        BlockCipherModesFactory<IV_Vector>::createBlockCipherMode(
          IV,
          std::bind(&THIS::processEncodeBlock, this, std::placeholders::_1))),
        rounds(round) {}
   
   virtual ~BlockCipher() { delete block_mode; }
   
   /* Check the key provided by the user and set it if correct. */
   virtual void setKey(const BytesVector &) = 0;
   
   /* Generate sub-keys from the key provided by the user when encoding. */
   virtual void generateSubkeys() = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual const DataType encodeBlock(const DataType &input) = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual const DataType decodeBlock(const DataType &input) = 0;
   
   /* Extract a vector of integers from the block of bytes. */
   const DataType getIntegersFromInputBlock(const BytesVector &block) const
   {
      return InputOutputBlockGetter<BytesVector, DataType, InputBlockSize, EndianType>::inputBlock(block);
   }
   
   /* Extract the bytes from the vector of integers and return the encoded / decoded block. */
   const BytesVector getOutputBlock(const DataType &int_block)
   {      
      return InputOutputBlockGetter<BytesVector, DataType, InputBlockSize, EndianType>::outputBlock(int_block);
   }
   
   /* Generate sub-keys from the key provided by the user when decoding. */
   virtual void generateInverseSubkeys()
   {
      generateSubkeys();
   }
   
   /* Pad 'data' with 'input_block_length' values given by 'fill_with'. */
   const BytesVector appendPadding(const BytesVector &input, const uint8_t fill_with) const
   {
      BytesVector padded_input(input);
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
   const BytesVector processEncodeBlock(const BytesVector &block)
   {
      DataType int_block = getIntegersFromInputBlock(block);
      int_block = encodeBlock(int_block);
      
      return getOutputBlock(int_block);
   }
   
   /* Decode an input block of bytes and return the output block. */
   const BytesVector processDecodeBlock(const BytesVector &block)
   {
      DataType int_block = getIntegersFromInputBlock(block);
      int_block = decodeBlock(int_block);
      
      return getOutputBlock(int_block);
   }
};

#endif