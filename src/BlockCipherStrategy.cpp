#include "BlockCipherStrategy.hpp"

#include "Tools.hpp"

/*
 * ECB Mode of encryption.
 */
const BlockCipherStrategy::Block 
BlockCipherECBStrategy::getCipherBlock(const Block &clear_block, const uint64_t&)
{
   return clear_block;
}

const BlockCipherStrategy::Block 
BlockCipherECBStrategy::getClearBlock(const Block &cipher_block, const uint64_t&)
{
   return cipher_block;
}

/*
 * CBC Mode of encryption.
 */
const BlockCipherStrategy::Block 
BlockCipherCBCStrategy::getCipherBlock(const Block &clear_block, const uint64_t &block_idx)
{
   const Block new_cipher_block = getXORedBlock(clear_block, getPreviousCipher(block_idx));
   old_cipher_block.push_back(new_cipher_block);  
   
   return new_cipher_block;
}

const BlockCipherStrategy::Block 
BlockCipherCBCStrategy::getClearBlock(const Block &cipher_block, const uint64_t &block_idx)
{
   return getXORedBlock(cipher_block, getPreviousCipher(block_idx));
}

const BlockCipherStrategy::Block
BlockCipherCBCStrategy::getPreviousCipher(const uint64_t &block_idx) const
{
   if (block_idx == 0)
   {
      return getIV();
   }

   return old_cipher_block[block_idx];
}