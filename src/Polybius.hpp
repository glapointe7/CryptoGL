
#ifndef POLYBIUS_HPP
#define POLYBIUS_HPP

#include "SquareCipher.hpp"

#include <string>

class Polybius : public SquareCipher
{
public:
   explicit Polybius(const KeyType &key) : SquareCipher(key) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
};

#endif