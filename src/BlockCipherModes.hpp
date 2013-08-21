
#ifndef BLOCKCIPHERMODES_HPP
#define BLOCKCIPHERMODES_HPP

#include "SymmetricCipher.hpp"
#include "BlockCipherOperationModes.hpp"

#include "exceptions/BadKeyLength.hpp"

class BlockCipherModes
{
protected:
   typedef SymmetricCipher::BytesContainer Block;
   using BadIVLength = BadKeyLength;
   
public:
   virtual ~BlockCipherModes() {}
   
   virtual const Block getCipherBlock(const Block &input_block) = 0;
   virtual const Block getClearBlock(const Block &input_block) = 0;
   
   //void setIV(const Block &IV) { this->IV = IV; }
   //const Block getIV() const { return IV; }
   
//private:
  // Block IV;
};

class BlockCipherECBMode : public BlockCipherModes
{
public:
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;
};

class BlockCipherCBCMode : public BlockCipherModes
{
public:
   BlockCipherCBCMode() {}
   explicit BlockCipherCBCMode(const Block &IV) : previous_cipher_block(IV) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;
   
private:
   Block previous_cipher_block;
};

/*class BlockCipherCFBMode : public BlockCipherModes
{
public:
   explicit BlockCipherCFBMode(const Block &IV) : previous_input_block(IV) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;

private:
   Block previous_input_block;
   Block previous_cipher_block;
};*/

class BlockCipherModesFactory
{
public:
   static BlockCipherModes* createBlockCipherMode(const OperationModes mode)
   {
      switch (mode)
      {
         case OperationModes::ECB: 
            return new BlockCipherECBMode();
         
         case OperationModes::CBC: 
            return new BlockCipherCBCMode();
         
         /*case OperationModes::CFB:  
            return new BlockCipherCFBMode();*/
            //case OperationModes::OFB:  return new BlockCipherOFBMode();
      }
      throw Exception("Accepted Modes are : ECB, CBC, CFB, OFB and CTR.");
   }
};

#endif