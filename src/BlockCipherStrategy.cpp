#include "BlockCipherStrategy.hpp"

#include "Tools.hpp"

/*
 * ECB Mode of encryption.
 */
const BlockCipherStrategy::Block BlockCipherECBStrategy::getCipherBlock(const Block &input_block)
{
   return input_block;
}

const BlockCipherStrategy::Block BlockCipherECBStrategy::getClearBlock(const Block &input_block)
{
   return input_block;
}

/*
 * CBC Mode of encryption.
 */
const BlockCipherStrategy::Block BlockCipherCBCStrategy::getCipherBlock(const Block &input_block)
{
   const Block new_cipher_block = getXORedBlock(input_block, previous_cipher_block);
   previous_cipher_block = new_cipher_block;  
   
   return new_cipher_block;
}

const BlockCipherStrategy::Block  BlockCipherCBCStrategy::getClearBlock(const Block &input_block)
{
   return getXORedBlock(input_block, previous_cipher_block);
}

/*
 * CFB Mode of encryption.
 */
/*const BlockCipherStrategy::Block BlockCipherCBCStrategy::getCipherBlock(const Block &input_block)
{
   const Block new_cipher_block = getXORedBlock(input_block, previous_cipher_block);
   previous_cipher_block = new_cipher_block;  
   
   return new_cipher_block;
}

const BlockCipherStrategy::Block  BlockCipherCBCStrategy::getClearBlock(const Block &input_block)
{
   return getXORedBlock(input_block, previous_cipher_block);
}*/