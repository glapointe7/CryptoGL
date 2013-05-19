
#ifndef BLOCKCIPHERECBSTRATEGY_HPP
#define BLOCKCIPHERECBSTRATEGY_HPP

#include "BlockCipherStrategy.hpp"

class BlockCipherECBStrategy : public BlockCipherStrategy
{
public:
   Block getCipherBlock(const Block &clearBlock, const uint32_t blockIdx);
   Block getClearBlock(const Block &cipherBlock, const uint32_t blockIdx);
};

#endif // BLOCKCIPHERECBSTRATEGY_HPP

