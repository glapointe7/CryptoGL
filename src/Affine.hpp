
#ifndef AFFINE_HPP
#define AFFINE_HPP

#include "StringCipher.hpp"

class Affine : public StringCipher
{
public:
   Affine(const int32_t a, const int32_t b) { setKey(a, b); }
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;
   
   void setKey(const int32_t a, const int32_t b);

private:
   int32_t a;
   int32_t b;
};

#endif