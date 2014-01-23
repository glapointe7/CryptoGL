
#ifndef MORSE_HPP
#define MORSE_HPP

#include "StringCipher.hpp"

#include <string>
#include <array>

class Morse : public StringCipher
{
public:
   Morse();

   ClassicalType encode(const ClassicalType &) override;
   ClassicalType decode(const ClassicalType &) override;

private:
   static const std::array<ClassicalType, 36> morse;
};

#endif