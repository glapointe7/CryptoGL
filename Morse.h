
#ifndef MORSE_H
#define MORSE_H

#include "StringCipher.h"

#include <string>
#include <vector>

class Morse : public StringCipher
{
public:
   Morse();

   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

private:
   std::vector<std::string> morse;
};

#endif