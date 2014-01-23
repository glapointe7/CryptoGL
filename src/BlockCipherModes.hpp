
#ifndef BLOCKCIPHERMODES_HPP
#define BLOCKCIPHERMODES_HPP

#include "Types.hpp"
#include "BigEndian.hpp"

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
   CTR      // Counter
};


class BlockCipherModes
{   
public:   
   using Block = BytesVector;
   using GetOutputBlockFunction = std::function<Block(const Block &)>;
   
   virtual ~BlockCipherModes() {}
   
   virtual Block getCipherBlock(const Block &input_block) = 0;
   virtual Block getClearBlock(const Block &input_block) = 0;
};

class BlockCipherECBMode : public BlockCipherModes
{
public:
   BlockCipherECBMode(const GetOutputBlockFunction &encode, const GetOutputBlockFunction &decode)
      : encode(encode), decode(decode) {}
   
   Block getCipherBlock(const Block &input_block) override;
   Block getClearBlock(const Block &input_block) override;
   
private:
   const GetOutputBlockFunction encode;
   const GetOutputBlockFunction decode;
};

class BlockCipherCBCMode : public BlockCipherModes
{
public:
   BlockCipherCBCMode(const Block &IV, const GetOutputBlockFunction &encode, const GetOutputBlockFunction &decode) 
      : previous_cipher_block(IV), encode(encode), decode(decode) {}
   
   Block getCipherBlock(const Block &input_block) override;
   Block getClearBlock(const Block &input_block) override;
   
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
   
   Block getCipherBlock(const Block &input_block) override;
   Block getClearBlock(const Block &input_block) override;

private:
   Block next_input_block;
   const GetOutputBlockFunction encode;
};

class BlockCipherOFBMode : public BlockCipherModes
{
public:   
   BlockCipherOFBMode(const Block &IV, const GetOutputBlockFunction &encode)
      : next_input_block(IV), encode(encode) {}
   
   Block getCipherBlock(const Block &input_block) override;
   Block getClearBlock(const Block &input_block) override;

private:
   Block next_input_block;
   const GetOutputBlockFunction encode;
};

class BlockCipherCTRMode : public BlockCipherModes
{
public:   
   BlockCipherCTRMode(const Block &IV, const GetOutputBlockFunction &encode)
      : new_IV(IV), encode(encode) 
   { 
      if(IV.size() == 16)
      {
         this->IV = Block(IV.begin(), IV.begin() + 8);
      }
      counter = BigEndian64::toInteger(Block(IV.begin() + IV.size() - 8, IV.end())); 
   }
   
   Block getCipherBlock(const Block &input_block) override;
   Block getClearBlock(const Block &input_block) override;

private:
   Block increaseCounter();
   
   uint64_t counter;
   Block new_IV;
   Block IV;
   const GetOutputBlockFunction encode;
};

template <uint8_t BlockSize>
class BlockCipherModesFactory
{
public:
   static BlockCipherModes* createBlockCipherMode(
      const OperationModes mode,
      const BlockCipherModes::Block &IV,
      const BlockCipherModes::GetOutputBlockFunction &encode,
      const BlockCipherModes::GetOutputBlockFunction &decode
   )
   {
      // Make sure the IV is compatible with the algorithm used for mode other than ECB.
      if(BlockSize != IV.size() && mode != OperationModes::ECB)
      {
         throw BadIVLength("The size of your IV has to be the same size as the block size", IV.size());
      }
      
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
            
         case OperationModes::CTR:  
            return new BlockCipherCTRMode(IV, encode);
      }
      throw Exception("Accepted Modes are : ECB, CBC, CFB, OFB and CTR.");
   }
};

#endif