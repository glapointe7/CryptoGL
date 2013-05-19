
#include "BlockCipher.hpp"

BlockCipher::BlockCipher(const OperationModes mode)
   : blockStrategy(BlockCipherStrategy::Factory(mode))
{}

BlockCipher::~BlockCipher()
{
   delete blockStrategy;
}

