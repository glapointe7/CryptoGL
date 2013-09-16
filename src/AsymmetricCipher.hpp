
#ifndef ASYMMETRICCIPHER_HPP
#define ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <vector>

#include "big_integers/BigIntegerLibrary.hh"

using BigIntVector = std::vector<BigInteger>;

class AsymmetricCipher : public Cipher<BigIntVector, BytesVector>
{   
protected:  
   virtual ~AsymmetricCipher() {}
   
   virtual const BigIntVector encode(const BytesVector &) = 0;
   virtual const BytesVector decode(const BigIntVector &) = 0;
   
   virtual void makePublicKey() = 0;
};

#endif