#ifndef RAILFENCE_HPP
#define RAILFENCE_HPP

#include "StringCipherWithPermutationKey.hpp"
#include <string>

class RedefenceZigzag : public StringCipherWithPermutationKey
{
public:
   RedefenceZigzag(const KeyType &key, const KeyType &key_levels);

   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setOffset(const uint32_t offset) { this->offset = offset; }
   
private:
   uint32_t offset = 0;
   uint32_t max_level;
   KeyType key_levels;
};

class Railfence : public RedefenceZigzag
{
public:
   explicit Railfence(const int32_t key)
      : RedefenceZigzag(KeyType(1, key), KeyType(1, key)) {}
};

class Redefence : public RedefenceZigzag
{
public:
   explicit Redefence(const KeyType &key)
      : RedefenceZigzag(key, KeyType(1, key.size())) {}
};

#endif