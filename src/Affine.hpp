
#ifndef AFFINE_HPP
#define AFFINE_HPP

#include "StringCipher.hpp"

class Affine : public StringCipher
{
public:
   Affine(const int32_t a, const int32_t b) { setKey(a, b); }
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setKey(const int32_t a, const int32_t b);

private:
   int32_t a;
   int32_t b;
};

#endif