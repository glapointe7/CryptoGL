
#ifndef STREAMCIPHER_HPP
#define STREAMCIPHER_HPP

#include "SymmetricCipher.hpp"

class StreamCipher : public SymmetricCipher
{
public:
   virtual const BytesVector decode(const BytesVector &cipher_message) final
   {
      return encode(cipher_message);
   }
   
protected:   
   virtual ~StreamCipher() {}

   virtual const BytesVector encode(const BytesVector &) = 0;
      
   virtual void setKey(const BytesVector &) = 0;
   virtual void generateSubkeys() = 0;
};

#endif