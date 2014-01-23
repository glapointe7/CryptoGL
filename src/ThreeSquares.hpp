
#ifndef THREESQUARES_HPP
#define THREESQUARES_HPP

#include "SquareCipher.hpp"

class ThreeSquares : public SquareCipher
{
public:
   ThreeSquares(const KeyType &key1, const KeyType &key2, const KeyType &key3);
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;
   
private:
   KeyType key2;
   KeyType key3;
};

#endif