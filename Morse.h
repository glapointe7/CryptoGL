#ifndef MORSE_H
#define MORSE_H

#include "StringCipher.h"

#include <string>
#include <vector>

class Morse : public StringCipher
{
public:
   Morse();
   ~Morse();

   string encode();
   string decode();

private:
   vector<string> morse;
};
#endif