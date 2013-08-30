
#ifndef CHAO_HPP
#define CHAO_HPP

#include "StringCipher.hpp"

class Chao : public StringCipher
{
public:
   Chao(const ClassicalType &left_alpha, const ClassicalType &right_alpha);
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
private:
   ClassicalType left_alpha;
   ClassicalType right_alpha;
   static const uint8_t nadir;
  
   void leftShiftAlphabet(const int8_t ch, const uint8_t index);
   void rightShiftAlphabet(const int8_t ch, const uint8_t index);
};

#endif