#include "BlockCipher.hpp"

void BlockCipher::setMode(const OpModes &Mode)
{
   this->Mode = Mode;
}

void BlockCipher::setIV(const BytesContainer &IV)
{
   if(IV.size() != 8)
   {
      throw BadIVLength("Your IV have to be 8 bytes length.", IV.size());
   }
   
   this->IV = IV;
}