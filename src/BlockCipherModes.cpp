#include "BlockCipherModes.hpp"

#include "Tools.hpp"

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
   const Block new_cipher_block = encode(Tools::getXORedBlock(input_block, previous_cipher_block));
   previous_cipher_block = new_cipher_block;  
   
   return new_cipher_block;
}

BlockCipherModes::Block BlockCipherCBCMode::getClearBlock(const Block &input_block)
{
   const Block clear_block = Tools::getXORedBlock(decode(input_block), previous_cipher_block);
   previous_cipher_block = input_block;
   
   return clear_block;
}

/*
 * CFB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherCFBMode::getCipherBlock(const Block &input_block)
{   
   const Block output = encode(next_input_block);
   const Block cipher = Tools::getXORedBlock(input_block, output);
   next_input_block = cipher;
   
   return cipher;
}

BlockCipherModes::Block  BlockCipherCFBMode::getClearBlock(const Block &input_block)
{
   const Block output = encode(next_input_block);
   const Block clear = Tools::getXORedBlock(input_block, output);
   next_input_block = input_block;
   
   return clear;
}

/*
 * OFB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherOFBMode::getCipherBlock(const Block &input_block)
{   
   const Block output = encode(next_input_block);
   const Block cipher = Tools::getXORedBlock(input_block, output);
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
   
   return Tools::getXORedBlock(input_block, output);
}

BlockCipherModes::Block  BlockCipherCTRMode::getClearBlock(const Block &input_block)
{
   return getCipherBlock(input_block);
}

BlockCipherModes::Block BlockCipherCTRMode::increaseCounter()
{
   ++counter;
   return Tools::mergeVectors(IV, BigEndian64::toBytesVector(counter));
}