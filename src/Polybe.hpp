
#ifndef POLYBE_HPP
#define POLYBE_HPP

#include "SquareCipher.hpp"

#include <string>

class Polybe : public SquareCipher
{
public:
   explicit Polybe(const KeyType &key) : SquareCipher(key) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
};

#endif