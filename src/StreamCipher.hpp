/*
 * Abstract class for Stream ciphers.
 */
#ifndef STREAMCIPHER_HPP
#define STREAMCIPHER_HPP

#include "SymmetricCipher.hpp"

template <class KeystreamType>
class StreamCipher : public SymmetricCipher
{
public:
   /* Since encoding is his own inverse, then decoding is simply a call to encode. */
   virtual BytesVector decode(const BytesVector &message) final
   {
      return encode(message);
   }
   
protected:
   virtual ~StreamCipher() {}

   virtual BytesVector encode(const BytesVector &) = 0;
   
   virtual KeystreamType generateKeystream() = 0;
   
   /* Set the key and check if the key has a correct length. 
      If yes, then it execute the keySetup. */
   virtual void setKey(const BytesVector &) = 0;
   
   virtual void keySetup() = 0;
};

#endif