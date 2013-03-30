
#ifndef VIGENERE_H
#define VIGENERE_H

#include "StringCipher.h"

class Vigenere : public StringCipher
{
public:
   Vigenere();
   
   std::string encode(const std::string &);
   std::string decode(const std::string &);
   
   void setKey(const std::string &key);
   
private:
   std::string key;
};

#endif // VIGENERE_H
