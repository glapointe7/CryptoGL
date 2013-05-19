
#ifndef STRINGCIPHERWITHSTRINGKEY_HPP
#define STRINGCIPHERWITHSTRINGKEY_HPP

#include "StringCipher.hpp"

#include <string>

class StringCipherWithStringKey : public StringCipher
{
public:
   typedef std::string KeyType;

   StringCipherWithStringKey();
   virtual ~StringCipherWithStringKey();

   void getKey() const { return key; }
   virtual void setKey(const KeyType &key);

private:
   KeyType key;
};

#endif // STRINGCIPHERWITHSTRINGKEY_HPP

