
#ifndef PLAYFAIR_HPP
#define PLAYFAIR_HPP

#include "SquareCipher.hpp"

#include <string>

class Playfair : public SquareCipher
{
public:
   Playfair(const KeyType &key) : SquareCipher(key) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
};

#endif