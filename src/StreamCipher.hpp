
#ifndef STREAMCIPHER_HPP
#define	STREAMCIPHER_HPP

#include "SymmetricCipher.hpp"

class StreamCipher : public SymmetricCipher
{
protected:   
   virtual ~StreamCipher() {}

   virtual const BytesVector encode(const BytesVector &) = 0;
   virtual const BytesVector decode(const BytesVector &) = 0;
   
   virtual void setKey(const BytesVector &) = 0;
   
   virtual void keySetup() = 0;
};

#endif