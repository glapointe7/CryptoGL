
#ifndef POLYBIUS_HPP
#define POLYBIUS_HPP

#include "SquareCipher.hpp"

#include <string>

class Polybius : public SquareCipher
{
public:
   Polybius(const KeyType &key, const ClassicalType &alpha) : SquareCipher(key, alpha) {}
   
   explicit Polybius(const KeyType &key) : Polybius(key, String::grid_uppercase_fr) {}
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;
};

#endif