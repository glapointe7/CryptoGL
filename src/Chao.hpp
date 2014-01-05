
#ifndef CHAO_HPP
#define CHAO_HPP

#include "StringCipher.hpp"

class Chao : public StringCipher
{
public:
   Chao(const ClassicalType &left_alpha, const ClassicalType &right_alpha);
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;
   
private:
   ClassicalType left_alpha;
   ClassicalType right_alpha;
   static constexpr uint8_t nadir = 13;
  
   void leftShiftAlphabet(const uint8_t index);
   void rightShiftAlphabet(const uint8_t index);
};

#endif