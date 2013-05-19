
#ifndef BLOCKCIPHERCBCSTRATEGY_HPP
#define BLOCKCIPHERCBCSTRATEGY_HPP

#include "BlockCipherStrategy.hpp"

class BlockCipherCBCStrategy : public BlockCipherStrategy
{
public:
   const Block getCipherBlock(const Block &clearBlock, const uint32_t blockIdx);
   const Block getClearBlock(const Block &cipherBlock, const uint32_t blockIdx);

private:
   const Block getPreviousCipher(const uint32_t blockIdx) const;

   std::vector<Block> oldCipherBlock;
};

#endif // BLOCKCIPHERCBCSTRATEGY_HPP

