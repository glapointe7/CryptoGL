
#ifndef BLOCKCIPHERSTRATEGY_HPP
#define BLOCKCIPHERSTRATEGY_HPP

#include "SymmetricCipher.hpp"
#include "BlockCipherOperationModes.hpp"

#include "exceptions/BadKeyLength.hpp"

class BlockCipherStrategy
{
protected:
   typedef SymmetricCipher::BytesContainer Block;
   using BadIVLength = BadKeyLength;
   
public:
   virtual ~BlockCipherStrategy() {}
   
   virtual const Block getCipherBlock(const Block &clear_block) = 0;
   virtual const Block getClearBlock(const Block &cipher_block) = 0;
   
   void setIV(const Block &IV) { this->IV = IV; }
   const Block getIV() const { return IV; }
   
private:
   Block IV;
};

class BlockCipherECBStrategy : public BlockCipherStrategy
{
public:
   virtual const Block getCipherBlock(const Block &clear_block) final;
   virtual const Block getClearBlock(const Block &cipher_block) final;
};

class BlockCipherCBCStrategy : public BlockCipherStrategy
{
public:
   BlockCipherCBCStrategy() : previous_cipher_block(getIV()) {}
   
   virtual const Block getCipherBlock(const Block &clear_block) final;
   virtual const Block getClearBlock(const Block &cipher_block) final;
   
private:
   Block previous_cipher_block;
};

/*class BlockCipherCFBStrategy : public BlockCipherStrategy
{
public:
   virtual const Block getCipherBlock(const Block &clear_block) final;
   virtual const Block getClearBlock(const Block &cipher_block) final;

private:
   const Block getPreviousCipher() const;

   Block previous_cipher_block;
};*/

class BlockCipherStrategyFactory
{
public:
   static BlockCipherStrategy* createBlockCipherStrategy(const OperationModes mode)
   {
      switch (mode)
      {
         case OperationModes::ECB: 
            return new BlockCipherECBStrategy();
         
         case OperationModes::CBC: 
            return new BlockCipherCBCStrategy();
         
         /*case OperationModes::CFB:  
            return new BlockCipherCFBStrategy();*/
            //case OperationModes::OFB:  return new BlockCipherOFBStrategy();
      }
      
      throw "Block Cipher : You gave a bad operation mode.";
   }
};

#endif