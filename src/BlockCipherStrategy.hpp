
#ifndef BLOCKCIPHERSTRATEGY_HPP
#define BLOCKCIPHERSTRATEGY_HPP

#include "SymmetricCipher.hpp"
#include "BlockCipherOperationModes.hpp"
#include "BlockCipherECBStrategy.hpp"
#include "BlockCipherCBCStrategy.hpp"
#include "BlockCipherPCBCStrategy.hpp"
#include "BlockCipherCFBStrategy.hpp"
#include "BlockCipherOFBStrategy.hpp"

class BlockCipherStrategy
{
public:
   virtual const Block getCipherBlock(const Block &clearBlock, const uint32_t blockIdx) = 0;
   virtual const Block getClearBlock(const Block &cipherBlock, const uint32_t blockIdx) = 0;

   static BlockCipherStrategy* Factory(const OperationModes mode)
   {
      switch (mode)
      {
      case ECB:  return new BlockCipherECBStrategy();
      case CBC:  return new BlockCipherCBCStrategy();
      case PCBC: return new BlockCipherPCBCStrategy();
      case CFB:  return new BlockCipherCFBStrategy();
      case OFB:  return new BlockCipherOFBStrategy();
      }
   }

protected:
   typedef SymmetricCipher::BytesContainer Block;
};

#endif // BLOCKCIPHERSTRATEGY_HPP

