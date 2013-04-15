#ifndef RAILFENCE_H
#define RAILFENCE_H

#include "StringCipher.h"
#include <string>

class Railfence : public StringCipher
{
public:
   Railfence();

   // Pour encoder avec un seul niveau pour tout le texte.
   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setKey(const unsigned int key);

private:
   unsigned int key;
};

#endif