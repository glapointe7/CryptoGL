/*
 * Symmetric cipher abstract class for modern cryptology.
 */
#ifndef SYMMETRICCIPHER_HPP
#define SYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <vector>

class SymmetricCipher : public Cipher<BytesVector>
{
protected:
   virtual ~SymmetricCipher() {}

   virtual const BytesVector encode(const BytesVector &) = 0;
   virtual const BytesVector decode(const BytesVector &) = 0;
   
   /* Check the key provided by the user and set it if correct. */
   virtual void setKey(const BytesVector &) = 0;
   
   BytesVector key;
};

#endif