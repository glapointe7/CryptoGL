
#ifndef MORSE_H
#define MORSE_H

#include "StringCipher.h"

#include <string>
#include <vector>

class Morse : public StringCipher
{
public:
   Morse();

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);

private:
   std::vector<std::string> morse;
};

#endif // MORSE_H
