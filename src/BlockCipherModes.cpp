#include "BlockCipherModes.hpp"

#include "Tools.hpp"

/*
 * ECB Mode of encryption.
 */
const BlockCipherModes::Block BlockCipherECBMode::getCipherBlock(const Block &input_block)
{
   return input_block;
}

const BlockCipherModes::Block BlockCipherECBMode::getClearBlock(const Block &input_block)
{
   return input_block;
}

/*
 * CBC Mode of encryption.
 */
const BlockCipherModes::Block BlockCipherCBCMode::getCipherBlock(const Block &input_block)
{
   const Block new_cipher_block = getXORedBlock(input_block, previous_cipher_block);
   previous_cipher_block = new_cipher_block;  
   
   return new_cipher_block;
}

const BlockCipherModes::Block  BlockCipherCBCMode::getClearBlock(const Block &input_block)
{
   return getXORedBlock(input_block, previous_cipher_block);
}

/*
 * CFB Mode of encryption.
 */
/*const BlockCipherModes::Block BlockCipherCBCMode::getCipherBlock(const Block &input_block)
{
   Block new_input_block(previous_input_block);
   new_input_block.reserve(previous_input_block.size() + previous_cipher_block.size());
   new_input_block.insert(new_input_block.end(), previous_cipher_block.begin(), previous_cipher_block.end());
   
   const Block output_block = block->processEncodeBlock(previous_input_block);
   previous_cipher_block = getXORedBlock(input_block, output_block);
   
   return previous_cipher_block;
}

const BlockCipherModes::Block  BlockCipherCBCMode::getClearBlock(const Block &input_block)
{
   return getCipherBlock(input_block);
}*/