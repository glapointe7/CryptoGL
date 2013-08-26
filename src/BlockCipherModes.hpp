
#ifndef BLOCKCIPHERMODES_HPP
#define BLOCKCIPHERMODES_HPP

#include "SymmetricCipher.hpp"
#include "BlockCipherOperationModes.hpp"

#include "exceptions/BadKeyLength.hpp"

#include <functional>

class BlockCipherModes
{
protected:
   typedef SymmetricCipher::BytesContainer Block;
   using BadIVLength = BadKeyLength;
   
public:   
   typedef typename std::function<const Block(const Block &)> GetOutputBlockFunction;
   virtual ~BlockCipherModes() {}
   
   virtual const Block getCipherBlock(const Block &input_block) = 0;
   virtual const Block getClearBlock(const Block &input_block) = 0;
};

class BlockCipherECBMode : public BlockCipherModes
{
public:
   BlockCipherECBMode(GetOutputBlockFunction encode, GetOutputBlockFunction decode)
      : encode(encode), decode(decode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;
   
private:
   GetOutputBlockFunction encode;
   GetOutputBlockFunction decode;
};

class BlockCipherCBCMode : public BlockCipherModes
{
public:
   BlockCipherCBCMode(const Block &IV, GetOutputBlockFunction encode, GetOutputBlockFunction decode) 
      : previous_cipher_block(IV), encode(encode), decode(decode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;
   
private:
   Block previous_cipher_block;
   GetOutputBlockFunction encode;
   GetOutputBlockFunction decode;
};

class BlockCipherCFBMode : public BlockCipherModes
{
public:   
   BlockCipherCFBMode(const Block &IV, GetOutputBlockFunction encode)
   : next_input_block(IV), encode(encode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;

private:
   Block next_input_block;
   GetOutputBlockFunction encode;
};

class BlockCipherModesFactory
{
public:
   static BlockCipherModes* createBlockCipherMode(
      const OperationModes mode,
      const SymmetricCipher::BytesContainer &IV,
      BlockCipherCFBMode::GetOutputBlockFunction encode,
      BlockCipherCFBMode::GetOutputBlockFunction decode)
   {
      switch (mode)
      {
         case OperationModes::ECB: 
            return new BlockCipherECBMode(encode, decode);
         
         case OperationModes::CBC: 
            return new BlockCipherCBCMode(IV, encode, decode);
         
         case OperationModes::CFB:  
            return new BlockCipherCFBMode(IV, encode);
            //case OperationModes::OFB:  return new BlockCipherOFBMode();
      }
      throw Exception("Accepted Modes are : ECB, CBC, CFB, OFB and CTR.");
   }
};

#endif