#ifndef RAILFENCE_HPP
#define RAILFENCE_HPP

#include "StringCipherWithPermutationKey.hpp"

#include <string>
#include <list>

class RedefenceZigzag : public StringCipherWithPermutationKey
{
public:
   RedefenceZigzag(const KeyType &key, const KeyType &key_levels);
   virtual ~RedefenceZigzag() {}

   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setOffset(const uint32_t offset) { this->offset = offset; }
   
private:
   /* Encode data block with the basic railfence cipher. */
   void addEncodedText(std::vector<ClassicalType> &rows, const ClassicalType &data, 
           const int32_t i, const int32_t level, const int32_t mod) const;
   
   /* Redefence : Replace each row following the permutation key. */
   ClassicalType swapRowsWithPermutationKey(const std::vector<ClassicalType> &rows, const uint32_t length) const;
   std::vector<std::list<int8_t> > getFirstDecoding(const ClassicalType &cipher_text, int32_t &last) const;
   
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