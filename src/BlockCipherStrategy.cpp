#include "BlockCipherStrategy.hpp"

#include "Tools.hpp"

/*
 * ECB Mode of encryption.
 */
const BlockCipherStrategy::Block BlockCipherECBStrategy::getCipherBlock(const Block &clear_block)
{
   return clear_block;
}

const BlockCipherStrategy::Block BlockCipherECBStrategy::getClearBlock(const Block &cipher_block)
{
   return cipher_block;
}

/*
 * CBC Mode of encryption.
 */
const BlockCipherStrategy::Block BlockCipherCBCStrategy::getCipherBlock(const Block &clear_block)
{
   const Block new_cipher_block = getXORedBlock(clear_block, previous_cipher_block);
   previous_cipher_block = new_cipher_block;  
   
   return new_cipher_block;
}

const BlockCipherStrategy::Block  BlockCipherCBCStrategy::getClearBlock(const Block &cipher_block)
{
   return getXORedBlock(cipher_block, previous_cipher_block);
}