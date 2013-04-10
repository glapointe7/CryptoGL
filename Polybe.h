
#ifndef POLYBE_H
#define POLYBE_H

#include "SquareCipher.h"

#include <string>

class Polybe : public SquareCipher
{
public:
   Polybe();

   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);
};

#endif // POLYBE_H
