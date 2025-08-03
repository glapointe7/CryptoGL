/*
 * GenerateSubkeys and its inverse should be done in the constructor. This would avoid useless IF.
 */
#pragma once

#include "SymmetricCipher.hpp"
#include "BlockCipherModes.hpp"
#include "Endian.hpp"
#include "Padding.hpp"
#include <functional>

namespace CryptoGL
{
    template <class BytesVector, class DataType, class EndianType>
    class InputOutputBlockGetter
    { 
    public:   
       static BytesVector outputBlock(const DataType &current_block)
       {  
          return EndianType::toBytesVector(current_block);
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
    static_assert(!(InputBlockSize % 8), "InputBlockSize has to be a multiple of 8.");

    public:   
       /* Encode a message block by block. */
       BytesVector encode(const BytesVector &message) override
       {
          generateSubkeys();

          const BytesVector message_padded = Padding::zeros(message, InputBlockSize);

          const uint64_t message_padded_len = message_padded.size();
          BytesVector output(message_padded_len);
          for (uint64_t n = 0; n < message_padded_len; n += InputBlockSize)
          {
             const BytesVector input_block = message_padded.range(n, n + InputBlockSize);
             const BytesVector encoded_block = block_mode->encodeBlock(input_block);
             output.extend(encoded_block);         
          }

          return output;
       }

       /* Decode a message block by block. */
       BytesVector decode(const BytesVector &message) override
       {
          generateInverseSubkeys();

          const uint64_t message_len = message.size();
          BytesVector output(message_len);
          for (uint64_t n = 0; n < message_len; n += InputBlockSize)
          {
             const BytesVector input_block = message.range(n, n + InputBlockSize);
             const BytesVector decoded_block = block_mode->decodeBlock(input_block);
             output.extend(decoded_block);
          }

          return output;
       }

       /* Encode an input block of bytes and return it with the right type. */
       BytesVector encodeCurrentBlock(const BytesVector &block)
       {
          if(subkeys.empty())
          {
             generateSubkeys();
          }

          current_block = getIntegersFromInputBlock(block);
          processEncodingCurrentBlock();

          return getOutputBlock();
       }

       /* Decode an input block of bytes and return it with the right type. */
       BytesVector decodeCurrentBlock(const BytesVector &block)
       {
          if(subkeys.empty())
          {
             generateInverseSubkeys();
          }

          current_block = getIntegersFromInputBlock(block);
          processDecodingCurrentBlock();

          return getOutputBlock();
       }

       /* Return the size of a block cipher. */
       static constexpr uint8_t getBlockSize() { return InputBlockSize; }

    protected:
       using SubkeysContainer = Vector<SubkeyType>;
       using THIS = BlockCipher<SubkeyType, DataType, InputBlockSize, EndianType>;

       /* Default constructor : Only for ECB, CBC, CFB, OFB and CTR modes. An IV is needed for 
        * CBC, CFB, OFB and CTR modes. For the ECB mode, IV is empty. */
       BlockCipher(const OperationModes mode, const uint8_t rounds, const BytesVector &IV)
          : block_mode(
            BlockCipherModesFactory<InputBlockSize>::createBlockCipherMode(
              mode,
              IV,
              std::bind(&THIS::encodeCurrentBlock, this, std::placeholders::_1),
              std::bind(&THIS::decodeCurrentBlock, this, std::placeholders::_1))),
            rounds(rounds) {}

       ~BlockCipher() override { delete block_mode; }
       
       /* Generate sub-keys from the key provided by the user when encoding. */
       virtual void generateSubkeys() = 0;

       /* Process encoding algorithm to the current block which is a vector of integers. */
       virtual void processEncodingCurrentBlock() = 0;

       /* Process decoding algorithm to the current block which is a vector of integers. */
       virtual void processDecodingCurrentBlock() = 0;

       /* Generate sub-keys from the key provided by the user when decoding. */
       virtual void generateInverseSubkeys()
       {
          generateSubkeys();
       }

       /* (Strategies) Modes used by a block cipher : ECB, CBC, CFB, OFB and CTR. */
       BlockCipherModes *block_mode;

       /* Number of rounds used by a block cipher algorithm. */
       uint8_t rounds;

       /* The subkeys created from the main key for the current block cipher. */
       SubkeysContainer subkeys;
       
       /* Current block to process encoding or decoding algorithm. */
       DataType current_block;

    private:
       /* Extract a vector of integers from the block of bytes. */
       static DataType getIntegersFromInputBlock(const BytesVector &block)
       {
          return InputOutputBlockGetter<BytesVector, DataType, EndianType>::inputBlock(block);
       }

       /* Extract the bytes from the vector of integers and return the encoded / decoded block. */
       BytesVector getOutputBlock() const
       {      
          return InputOutputBlockGetter<BytesVector, DataType, EndianType>::outputBlock(current_block);
       }
    };
}