
#ifndef ASYMMETRICCIPHER_HPP
#define ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <vector>
#include <stdint.h>

#include "big_integers/BigIntegerLibrary.hh"

typedef std::vector<BigInteger> BigIntVector;

class AsymmetricCipher : public Cipher<BigIntVector, BytesVector>
{   
protected:  
   virtual ~AsymmetricCipher() {}
   
   virtual const BigIntVector encode(const BytesVector &) = 0;
   virtual const BytesVector decode(const BigIntVector &) = 0;
   
   virtual void makePublicKey() = 0;
};

#endif