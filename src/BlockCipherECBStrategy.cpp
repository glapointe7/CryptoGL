
#include "BlockCipherECBStrategy.hpp"

BlockCipherECBStrategy::Block
BlockCipherECBStrategy::getCipherBlock(const Block &clearBlock, const uint32_t /*blockIdx*/)
{
   return clearBlock;
}

BlockCipherECBStrategy::Block
BlockCipherECBStrategy::getClearBlock(const Block &cipherBlock, const uint32_t /*blockIdx*/)
{
   return cipherBlock;
}

