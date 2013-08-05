#ifndef BLOCKCIPHER_HPP
#define BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "BlockCipherOperationModes.hpp"
#include "BlockCipherStrategy.hpp"

class BlockCipher : public SymmetricCipher
{     
public:
   /* Set an IV for all modes except ECB. */
   void setIV(const BytesContainer &IV);
   
protected:
   typedef std::vector<uint32_t> UInt32Container;
   typedef std::vector<uint64_t> UInt64Container;
   
   BlockCipher(const OperationModes mode, const uint8_t block_length)
      : block_strategy(BlockCipherStrategyFactory::createBlockCipherStrategy(mode)),
        input_block_length(block_length) {}
   virtual ~BlockCipher() { delete block_strategy; }
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void setKey(const BytesContainer &) = 0;
   virtual void generateSubkeys() = 0;
   virtual void generateInverseSubkeys();
   
   virtual const BytesContainer getOutputBlock(const BytesContainer &data, const bool to_encode) = 0;
   //virtual const BytesContainer getDecodedBlock(const BytesContainer &data);
   //virtual const BytesContainer getEncodedBlock(const BytesContainer &data);
   
   /* Process general encoding / decoding for block ciphers. */
   const BytesContainer processEncoding(const BytesContainer &data);
   const BytesContainer processDecoding(const BytesContainer &data);
   
   /* Pad 'data' with 'input_block_length' values given by 'fill_with'. */
   const BytesContainer appendPadding(const BytesContainer &input, const uint8_t fill_with) const;

   BlockCipherStrategy *block_strategy;
   const uint8_t input_block_length;
   UInt64Container subkeys;
};

#endif