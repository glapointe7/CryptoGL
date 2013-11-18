
#ifndef MORSE_HPP
#define MORSE_HPP

#include "StringCipher.hpp"

#include <string>
#include <array>

class Morse : public StringCipher
{
public:
   Morse();

   virtual ClassicalType encode(const ClassicalType &);
   virtual ClassicalType decode(const ClassicalType &);

private:
   static const std::array<ClassicalType, 36> morse;
};

#endif