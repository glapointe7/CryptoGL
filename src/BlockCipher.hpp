
#ifndef BLOCKCIPHER_HPP
#define BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "BlockCipherModes.hpp"

#include "BigEndian.hpp"
#include "LittleEndian.hpp"
#include "Padding.hpp"

#include <functional>

template <class BytesVector, class DataType, class EndianType>
class InputOutputBlockGetter
{ 
public:   
   static BytesVector outputBlock(const DataType &int_block)
   {  
      return EndianType::toBytesVector(int_block);
   }
   
   static DataType inputBlock(const BytesVector &block)
   {  
      return EndianType::toIntegersVector(block);
   }
};

template <class BytesVector, class EndianType>
class InputOutputBlockGetter<BytesVector, uint64_t, EndianType>
{
public:
   static BytesVector outputBlock(const uint64_t &int_block)
   {
      return EndianType::toBytesVector(int_block);
   }
   
   static uint64_t inputBlock(const BytesVector &block)
   {
      return EndianType::toInteger(block);
   }
};

template <class SubkeyType, class DataType, uint8_t InputBlockSize, class EndianType>
class BlockCipher : public SymmetricCipher
{   
static_assert(!(InputBlockSize & 7), "InputBlockSize has to be a multiple of 8.");

public:   
   /* Process general encoding for block ciphers. */
   BytesVector encode(const BytesVector &message)
   {
      const BytesVector message_padded = Padding::zeros(message, InputBlockSize);

      generateSubkeys();

      const uint64_t message_padded_len = message_padded.size();
      BytesVector output;
      output.reserve(message_padded_len);
      for (uint64_t n = 0; n < message_padded_len; n += InputBlockSize)
      {
         const BytesVector input_block(message_padded.begin() + n, message_padded.begin() + n + InputBlockSize);
         const BytesVector encoded_block = block_mode->getCipherBlock(input_block);
         output.insert(output.end(), encoded_block.begin(), encoded_block.end());
      }

      return output;
   }
   
   /* Process general decoding for block ciphers. */
   BytesVector decode(const BytesVector &message)
   {
      generateInverseSubkeys();

      const uint64_t message_len = message.size();
      BytesVector output;
      output.reserve(message_len);
      for (uint64_t n = 0; n < message_len; n += InputBlockSize)
      {
         const BytesVector input_block(message.begin() + n, message.begin() + n + InputBlockSize);
         const BytesVector decoded_block = block_mode->getClearBlock(input_block);
         output.insert(output.end(), decoded_block.begin(), decoded_block.end());
      }

      return output;
   }
   
   /* Encode an input block of bytes and return the output block. */
   BytesVector processEncodeBlock(const BytesVector &block)
   {
      DataType int_block = getIntegersFromInputBlock(block);
      int_block = encodeBlock(int_block);
      
      return getOutputBlock(int_block);
   }
   
   /* Decode an input block of bytes and return the output block. */
   BytesVector processDecodeBlock(const BytesVector &block)
   {
      DataType int_block = getIntegersFromInputBlock(block);
      int_block = decodeBlock(int_block);
      
      return getOutputBlock(int_block);
   }
   
   /* Return the size of a block cipher. */
   static constexpr uint8_t getBlockSize() { return InputBlockSize; }
   
protected:
   using SubkeysContainer = std::vector<SubkeyType>;
   using THIS = BlockCipher<SubkeyType, DataType, InputBlockSize, EndianType>;
   
   /* Default constructor : Only for ECB, CBC, CFB, OFB and CTR modes. An IV is needed for 
    * CBC, CFB, OFB and CTR modes. For the ECB mode, IV is empty. */
   BlockCipher(const OperationModes mode, const uint8_t rounds, const BytesVector &IV)
      : block_mode(
        BlockCipherModesFactory<InputBlockSize>::createBlockCipherMode(
          mode,
          IV,
          std::bind(&THIS::processEncodeBlock, this, std::placeholders::_1),
          std::bind(&THIS::processDecodeBlock, this, std::placeholders::_1))),
        rounds(rounds) {}
   
   virtual ~BlockCipher() { delete block_mode; }
   
   /* Check the key provided by the user and set it if correct. */
   virtual void setKey(const BytesVector &) = 0;
   
   /* Generate sub-keys from the key provided by the user when encoding. */
   virtual void generateSubkeys() = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual DataType encodeBlock(const DataType &input) = 0;
   
   /* Encode the input block provided as a vector of integers. */
   virtual DataType decodeBlock(const DataType &input) = 0;
      
   /* Generate sub-keys from the key provided by the user when decoding. */
   virtual void generateInverseSubkeys()
   {
      generateSubkeys();
   }
   
   /* (Strategy) Modes used by a block cipher : ECB, CBC, CFB, OFB and CTR. */
   BlockCipherModes *block_mode;
   
   /* Number of rounds used by a block cipher algorithm. */
   uint8_t rounds;
   SubkeysContainer subkeys;
   
private:
   /* Extract a vector of integers from the block of bytes. */
   static DataType getIntegersFromInputBlock(const BytesVector &block)
   {
      return InputOutputBlockGetter<BytesVector, DataType, EndianType>::inputBlock(block);
   }
   
   /* Extract the bytes from the vector of integers and return the encoded / decoded block. */
   static BytesVector getOutputBlock(const DataType &int_block)
   {      
      return InputOutputBlockGetter<BytesVector, DataType, EndianType>::outputBlock(int_block);
   }
};

#endif