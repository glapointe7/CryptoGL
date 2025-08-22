/*
 * GenerateSubkeys and its inverse should be done in the constructor. This would avoid useless IF.
 */
#pragma once

#include "../../common/SymmetricCipher.hpp"
#include "BlockCipherModes.hpp"
#include "../../../core/Utils/Endian.hpp"
#include "../../../core/Utils/Padding.hpp"
#include <functional>

namespace CryptoGL
{
   template <typename BytesVector, typename BlockType, typename EndianType>
   class InputOutputBlockGetter
   {
   public:
      static BytesVector outputBlock(const BlockType &current_block)
      {
         return EndianType::toBytesVector(current_block);
      }

      static BlockType inputBlock(const BytesVector &block)
      {
         return EndianType::toIntegersVector(block);
      }
   };

   template <typename BytesVector, typename EndianType>
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

   template <typename SubkeyType, typename InputBlockType, uint8_t InputBlockSize, typename EndianType>
   class BlockCipher : public SymmetricCipher<BytesVector>
   {
      //static_assert(std::is_array_v<InputBlockType>, "InputBlockType must be Array<T, N>");
      //static_assert(InputBlockType::size() * sizeof(typename InputBlockType::value_type) == InputBlockSize,
      //            "Array size must match InputBlockSize");

   public:
      /* Encode a message block by block sequentially */
      BytesVector encode(const BytesVector &message) override
      {
         generateSubkeys();

         const BytesVector message_padded = Padding::zeros(message, InputBlockSize);

         const uint64_t message_padded_len = message_padded.size();
         BytesVector output;
         output.reserve(message_padded_len); 
         for (uint64_t n = 0; n < message_padded_len; n += InputBlockSize)
         {
            const BytesVector input_block = message_padded.range(n, n + InputBlockSize);
            const BytesVector encoded_block = block_mode->encodeBlock(input_block);
            output.extend(encoded_block);
         }

         return output;
      }

      // Encode using bitslice method meaning that for a block of 4 words of 32-bits,
      // we encode 32 blocks of 128 bits in parallel.
      /*BytesVector encodeUsingBitSlice(const BytesVector &message) override
      {
         generateSubkeys();

         const BytesVector message_padded = Padding::zeros(message, InputBlockSize);
         const uint64_t message_padded_len = message_padded.size();
         BytesVector output(message_padded_len);
         constexpr size_t BITSLICE_SIZE = sizeof(WordType) * 8 * InputBlockSize;

         for (uint64_t n = 0; n < message_padded_len; n += BITSLICE_SIZE)
         {
            const BytesVector input_block = message_padded.range(n, n + BITSLICE_SIZE);
            const BytesVector encoded_block = block_mode->encodeBlock(input_block);
            output.extend(encoded_block);
         }

         return output;
      }*/



      /* Decode a message block by block sequentially */
      BytesVector decode(const BytesVector &message) override
      {
         generateInverseSubkeys();

         const uint64_t message_len = message.size();
         BytesVector output;
         output.reserve(message_len);
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
         if (subkeys.empty())
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
         if (subkeys.empty())
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
      using THIS = BlockCipher<SubkeyType, InputBlockType, InputBlockSize, EndianType>;

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
      InputBlockType current_block;

   private:
      /* Extract a vector of integers from the block of bytes. */
      static InputBlockType getIntegersFromInputBlock(const BytesVector &block)
      {
         return InputOutputBlockGetter<BytesVector, InputBlockType, EndianType>::inputBlock(block);
      }

      /* Extract the bytes from the vector of integers and return the encoded / decoded block. */
      BytesVector getOutputBlock() const
      {
         return InputOutputBlockGetter<BytesVector, InputBlockType, EndianType>::outputBlock(current_block);
      }
   };
}