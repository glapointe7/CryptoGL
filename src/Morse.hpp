
#ifndef MORSE_HPP
#define MORSE_HPP

#include "StringCipher.hpp"

#include <string>
#include <vector>

class Morse : public StringCipher
{
public:
   Morse();

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

private:
   static const std::vector<std::string> morse;
};

#endif