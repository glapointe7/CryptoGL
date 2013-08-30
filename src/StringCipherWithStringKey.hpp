/*
 * Manage String cipher keys of String type.
 */
#ifndef STRINGCIPHERWITHSTRINGKEY_HPP
#define STRINGCIPHERWITHSTRINGKEY_HPP

#include "StringCipher.hpp"

#include <string>
#include <vector>

class StringCipherWithStringKey : public StringCipher
{
public:
   typedef ClassicalType KeyType;

   const KeyType getKey() const { return key; }
   virtual void setKey(const KeyType &key);

protected:
   StringCipherWithStringKey() : key("") {}
   virtual ~StringCipherWithStringKey() {}
   
   void checkKey(const KeyType &key) const;
   
private:   
   KeyType key;
};

#endif