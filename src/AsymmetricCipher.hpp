
#ifndef ASYMMETRICCIPHER_HPP
#define	ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <vector>
#include <stdint.h>

#include "big_integers/BigIntegerLibrary.hh"

class AsymmetricCipher : public Cipher<std::vector<BigInteger>, std::vector<uint8_t> >
{
public:      
   typedef std::vector<BigInteger> BigIntVector;
   typedef std::vector<uint8_t> BytesContainer;
   
protected:  
   virtual ~AsymmetricCipher() {}
   
   virtual const BigIntVector encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BigIntVector &) = 0;
   
   virtual void makePublicKey() = 0;
};

#endif