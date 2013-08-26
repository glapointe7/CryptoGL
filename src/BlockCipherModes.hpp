
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
   BlockCipherECBMode(const GetOutputBlockFunction &encode, const GetOutputBlockFunction &decode)
      : encode(encode), decode(decode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;
   
private:
   const GetOutputBlockFunction encode;
   const GetOutputBlockFunction decode;
};

class BlockCipherCBCMode : public BlockCipherModes
{
public:
   BlockCipherCBCMode(const Block &IV, const GetOutputBlockFunction &encode, const GetOutputBlockFunction &decode) 
      : previous_cipher_block(IV), encode(encode), decode(decode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;
   
private:
   Block previous_cipher_block;
   const GetOutputBlockFunction encode;
   const GetOutputBlockFunction decode;
};

class BlockCipherCFBMode : public BlockCipherModes
{
public:   
   BlockCipherCFBMode(const Block &IV, const GetOutputBlockFunction &encode)
      : next_input_block(IV), encode(encode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;

private:
   Block next_input_block;
   const GetOutputBlockFunction encode;
};

class BlockCipherOFBMode : public BlockCipherModes
{
public:   
   BlockCipherOFBMode(const Block &IV, const GetOutputBlockFunction &encode)
      : next_input_block(IV), encode(encode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;

private:
   Block next_input_block;
   const GetOutputBlockFunction encode;
};

class BlockCipherCTRMode : public BlockCipherModes
{
public:   
   explicit BlockCipherCTRMode(const SymmetricCipher::IVContainer &IV, const GetOutputBlockFunction &encode)
      : IV(IV), encode(encode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;

private:
   uint64_t block_index = 0;
   const SymmetricCipher::IVContainer IV;
   const GetOutputBlockFunction encode;
};

template <class Block>
class BlockCipherModesFactory
{
public:
   static BlockCipherModes* createBlockCipherMode(
      const OperationModes mode,
      const Block &IV,
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
            
         case OperationModes::OFB:  
            return new BlockCipherOFBMode(IV, encode);
      }
      throw Exception("Accepted Modes are : ECB, CBC, CFB and OFB.");
   }
};

template<>
class BlockCipherModesFactory<SymmetricCipher::IVContainer>
{
public:
   static BlockCipherModes* createBlockCipherMode(
      const SymmetricCipher::IVContainer &IV,
      BlockCipherCFBMode::GetOutputBlockFunction encode)
   { 
      return new BlockCipherCTRMode(IV, encode);
   }
};

#endif