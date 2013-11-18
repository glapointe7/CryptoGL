
#ifndef STREAMCIPHER_HPP
#define STREAMCIPHER_HPP

#include "SymmetricCipher.hpp"

template <class KeystreamType>
class StreamCipher : public SymmetricCipher
{
public:
   virtual BytesVector decode(const BytesVector &cipher_message) final
   {
      return encode(cipher_message);
   }
   
protected:
   virtual ~StreamCipher() {}

   virtual BytesVector encode(const BytesVector &) = 0;
   
   virtual KeystreamType generateKeystream() = 0;
   
   /* Set the key and check if the key has a correct length. */
   virtual void setKey(const BytesVector &) = 0;
   
   virtual void keySetup() = 0;
};

#endif