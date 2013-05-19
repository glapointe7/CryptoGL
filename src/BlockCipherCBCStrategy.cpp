
#include "BlockCipherCBCStrategy.hpp"

const BlockCipherCBCStrategy::Block
BlockCipherCBCStrategy::getCipherBlock(const Block &clearBlock, const uint32_t blockIdx)
{
   const Block newCipherBlock =  clearBlock ^ getPreviousCipher(blockIdx);
   oldCipherBlock.push_back(newCipherBlock);  
   return newCipherBlock;
}

const BlockCipherCBCStrategy::Block
BlockCipherCBCStrategy::getClearBlock(const Block &cipherBlock, const uint32_t blockIdx)
{
   return cipherBlock ^ getPreviousCipher(blockIdx);
}

const BlockCipherCBCStrategy::Block
BlockCipherCBCStrategy::getPreviousCipher(const uint32_t blockIdx) const
{
   if (blockIdx == 0)
   {
      return getIV();
   }
   return oldCipherBlock[blockIdx - 1];
}

