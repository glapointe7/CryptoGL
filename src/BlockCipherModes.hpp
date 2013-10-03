
#ifndef BLOCKCIPHERMODES_HPP
#define BLOCKCIPHERMODES_HPP

#include "Types.hpp"

#include "exceptions/BadKeyLength.hpp"

#include <functional>

/* MODES OF OPERATION
 * 
 * ECB : C_i = encode(P_i).
 *       P_i = decode(C_i).
 *
 * CBC : C_i = encode(P_i ^ C_i-1) with IV = C_0.
 *       P_i = decode(C_i) ^ C_i-1 with IV = C_0.
 * 
 * CFB : C_i = encode(C_i-1) ^ P_i with IV = C_0.
 *       P_i = encode(C_i-1) ^ C_i ^ C_i with IV = C_0.
 * 
 * OFB : C_i = P_i ^ O_i with IV = I_0 and O_i = encode(I_i) and I_i = O_i-1.
 *       P_i = C_i ^ O_i with IV = I_0.and O_i = encode(I_i) and I_i = O_i-1.
 */

// Modes of operation to encode / decode a block cipher.
// Default to ECB mode.
enum class OperationModes : uint8_t
{
   ECB,     // electronic codebook
   CBC,     // Cipher-block chaining
   CFB,     // Cipher feedback
   OFB,     // Output feedback
   //CTR      // Counter
};



class BlockCipherModes
{
protected:
   using Block = BytesVector;
   
public:   
   using GetOutputBlockFunction = std::function<const Block(const Block &)>;
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
   explicit BlockCipherCTRMode(const IV_Vector &IV, const GetOutputBlockFunction &encode)
      : IV(IV), encode(encode) {}
   
   virtual const Block getCipherBlock(const Block &input_block) final;
   virtual const Block getClearBlock(const Block &input_block) final;

private:
   uint64_t block_index = 0;
   const IV_Vector IV;
   const GetOutputBlockFunction encode;
};

template <class Block>
class BlockCipherModesFactory
{
public:
   static BlockCipherModes* createBlockCipherMode(
      const OperationModes mode,
      const Block &IV,
      const BlockCipherCFBMode::GetOutputBlockFunction &encode,
      const BlockCipherCFBMode::GetOutputBlockFunction &decode)
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
class BlockCipherModesFactory<IV_Vector>
{
public:
   static BlockCipherModes* createBlockCipherMode(
      const IV_Vector &IV,
      const BlockCipherCFBMode::GetOutputBlockFunction &encode)
   { 
      return new BlockCipherCTRMode(IV, encode);
   }
};

#endif