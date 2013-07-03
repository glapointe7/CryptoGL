
#ifndef STREAMCIPHER_HPP
#define	STREAMCIPHER_HPP

#include "SymmetricCipher.hpp"

class StreamCipher : public SymmetricCipher
{
protected:
   typedef std::vector<uint32_t> UInt32Container;
   typedef std::vector<uint16_t> UInt16Container;
   
   virtual ~StreamCipher() {}

   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void keySetup() = 0;
};

#endif