#include "BlockCipherModes.hpp"

#include "Tools.hpp"

/*
 * ECB Mode of encryption.
 */
const BlockCipherModes::Block BlockCipherECBMode::getCipherBlock(const Block &input_block)
{
   return encode(input_block);
}

const BlockCipherModes::Block BlockCipherECBMode::getClearBlock(const Block &input_block)
{
   return decode(input_block);
}

/*
 * CBC Mode of encryption.
 */
const BlockCipherModes::Block BlockCipherCBCMode::getCipherBlock(const Block &input_block)
{
   const Block new_cipher_block = encode(getXORedBlock(input_block, previous_cipher_block));
   previous_cipher_block = new_cipher_block;  
   
   return new_cipher_block;
}

const BlockCipherModes::Block BlockCipherCBCMode::getClearBlock(const Block &input_block)
{
   const Block clear_block = getXORedBlock(decode(input_block), previous_cipher_block);
   previous_cipher_block = input_block;
   
   return clear_block;
}

/*
 * CFB Mode of encryption.
 */
const BlockCipherModes::Block BlockCipherCFBMode::getCipherBlock(const Block &input_block)
{   
   const Block output = encode(next_input_block);
   const Block cipher = getXORedBlock(input_block, output);
   next_input_block = cipher;
   
   return cipher;
}

const BlockCipherModes::Block  BlockCipherCFBMode::getClearBlock(const Block &input_block)
{
   const Block output = encode(next_input_block);
   const Block clear = getXORedBlock(input_block, output);
   next_input_block = input_block;
   
   return clear;
}