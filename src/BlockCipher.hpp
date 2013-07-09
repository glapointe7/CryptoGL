
#ifndef BLOCKCIPHER_HPP
#define BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "BlockCipherOperationModes.hpp"
#include "BlockCipherStrategy.hpp"

class BlockCipher : public SymmetricCipher
{     
protected:
   typedef std::vector<uint32_t> UInt32Container;
   typedef std::vector<uint64_t> UInt64Container;
   
   explicit BlockCipher(const OperationModes mode)
      : block_strategy(BlockCipherStrategyFactory::createBlockCipherStrategy(mode)) {}
   virtual ~BlockCipher() { delete block_strategy; }
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void setKey(const BytesContainer &) = 0;
   virtual void generateSubkeys() = 0;
   virtual const BytesContainer getOutputBlock(const BytesContainer &data, const bool to_encode) = 0;
   
   /* Process general encoding / decoding for block ciphers. */
   const BytesContainer process(const BytesContainer &data, const uint8_t block_len, const bool to_encode);
   
   /* Pad 'data' with 'block_length' values given by 'fill_with'. */
   static const BytesContainer addPadding(const BytesContainer &data, const uint32_t block_length, const uint8_t fill_with);

   BlockCipherStrategy *block_strategy;
   UInt64Container subkeys;
};

#endif