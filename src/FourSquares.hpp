
#ifndef FOURSQUARES_HPP
#define	FOURSQUARES_HPP

#include "SquareCipher.hpp"

class FourSquares : public SquareCipher
{
public:
   FourSquares(const KeyType &key1, const KeyType &key2);
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

private:
   KeyType key2;
};

#endif