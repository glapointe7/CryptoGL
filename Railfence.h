#ifndef RAILFENCE_H
#define RAILFENCE_H

#include "StringCipher.h"
#include <string>

class Railfence : public StringCipher
{
public:
   Railfence();

   // Pour encoder avec un seul niveau pour tout le texte.
   std::string encode(const std::string &);
   std::string decode(const std::string &);

   void setKey(const unsigned int key);

private:
   unsigned int key;
};
#endif