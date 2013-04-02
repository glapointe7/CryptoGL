
#ifndef POLYBE_H
#define POLYBE_H

#include "SquareCipher.h"

#include <string>

class Polybe : public SquareCipher
{
public:
   Polybe();

   std::string encode(const std::string &);
   std::string decode(const std::string &);

private:
   
};

#endif // POLYBE_H
