
#ifndef THREESQUARES_HPP
#define	THREESQUARES_HPP

#include "SquareCipher.hpp"

class ThreeSquares : public SquareCipher
{
public:
   ThreeSquares(const KeyType &key1, const KeyType &key2, const KeyType &key3);
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
private:
   KeyType key2;
   KeyType key3;
};

#endif