
#ifndef WOLSELEY_HPP
#define WOLSELEY_HPP

#include "StringCipherWithStringKey.hpp"

#include <string>

class Wolseley : public StringCipherWithStringKey
{
public:
   explicit Wolseley(const KeyType &key);

   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;
};

#endif