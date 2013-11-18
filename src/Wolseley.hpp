
#ifndef WOLSELEY_HPP
#define WOLSELEY_HPP

#include "StringCipherWithStringKey.hpp"

#include <string>

class Wolseley : public StringCipherWithStringKey
{
public:
   explicit Wolseley(const KeyType &key);

   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;
};

#endif