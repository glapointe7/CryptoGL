#include "BlockCipherModes.hpp"

#include "Vector.hpp"

using namespace CryptoGL;

/*
 * ECB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherECBMode::getCipherBlock(const Block &input_block)
{
   return encode(input_block);
}

BlockCipherModes::Block BlockCipherECBMode::getClearBlock(const Block &input_block)
{
   return decode(input_block);
}

/*
 * CBC Mode of encryption.
 */
BlockCipherModes::Block BlockCipherCBCMode::getCipherBlock(const Block &input_block)
{
   const Block new_cipher_block = encode(input_block.Xor(previous_cipher_block));
   previous_cipher_block = new_cipher_block;  
   
   return new_cipher_block;
}

BlockCipherModes::Block BlockCipherCBCMode::getClearBlock(const Block &input_block)
{
   const Block clear_block = decode(input_block).Xor(previous_cipher_block);
   previous_cipher_block = input_block;
   
   return clear_block;
}

/*
 * CFB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherCFBMode::getCipherBlock(const Block &input_block)
{   
   const Block output = encode(next_input_block);
   const Block cipher = input_block.Xor(output);
   next_input_block = cipher;
   
   return cipher;
}

BlockCipherModes::Block  BlockCipherCFBMode::getClearBlock(const Block &input_block)
{
   const Block output = encode(next_input_block);
   const Block clear = input_block.Xor(output);
   next_input_block = input_block;
   
   return clear;
}

/*
 * OFB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherOFBMode::getCipherBlock(const Block &input_block)
{   
   const Block output = encode(next_input_block);
   const Block cipher = input_block.Xor(output);
   next_input_block = output;
   
   return cipher;
}

BlockCipherModes::Block  BlockCipherOFBMode::getClearBlock(const Block &input_block)
{
   return getCipherBlock(input_block);
}

/*
 * CTR Mode of encryption.
 */
BlockCipherModes::Block BlockCipherCTRMode::getCipherBlock(const Block &input_block)
{   
   const Block output = encode(new_IV);
   new_IV = increaseCounter();
   
   return input_block.Xor(output);
}

BlockCipherModes::Block  BlockCipherCTRMode::getClearBlock(const Block &input_block)
{
   return getCipherBlock(input_block);
}

BlockCipherModes::Block BlockCipherCTRMode::increaseCounter()
{
   ++counter;
   Block tmpIV = IV;
   tmpIV.extend(BigEndian64::toBytesVector(counter));
   return tmpIV;
}