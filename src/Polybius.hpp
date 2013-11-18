
#ifndef POLYBIUS_HPP
#define POLYBIUS_HPP

#include "SquareCipher.hpp"

#include <string>

class Polybius : public SquareCipher
{
public:
   explicit Polybius(const KeyType &key) : SquareCipher(key) {}
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;
};

#endif