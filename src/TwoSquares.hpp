
#ifndef TWOSQUARES_HPP
#define	TWOSQUARES_HPP

#include "SquareCipher.hpp"

class TwoSquares : public SquareCipher
{
public:
   TwoSquares(const KeyType &key1, const KeyType &key2);
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

private:
   KeyType key2;
};

#endif