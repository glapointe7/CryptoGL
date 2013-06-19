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
   typedef std::string KeyType;
   //typedef std::vector<KeyType> KeysContainer;

   StringCipherWithStringKey() : key("") {}
   virtual ~StringCipherWithStringKey() {}

   const KeyType getKey() const { return key; }
   virtual void setKey(const KeyType &key);
   
   //void addKey(const KeyType &new_key);

protected:
   void checkKey(const KeyType &key) const;
   
private:   
   KeyType key;
};

#endif