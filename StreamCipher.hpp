
#ifndef STREAMCIPHER_HPP
#define	STREAMCIPHER_HPP

#include "SymmetricCipher.hpp"

class StreamCipher : public SymmetricCipher
{
public:
   virtual ~StreamCipher() {}
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void setKey(const BytesContainer &) = 0;
   virtual void initialize() = 0;
};

#endif

