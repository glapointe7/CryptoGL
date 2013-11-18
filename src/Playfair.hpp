
#ifndef PLAYFAIR_HPP
#define PLAYFAIR_HPP

#include "SquareCipher.hpp"

#include <string>

class Playfair : public SquareCipher
{
public:
   explicit Playfair(const KeyType &key) : SquareCipher(key) {}
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;
};

#endif