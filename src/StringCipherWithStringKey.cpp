
#include "StringCipherWithStringKey.hpp"

#include "exceptions/BadChar.hpp"
#include "exceptions/EmptyKey.hpp"

StringCipherWithStringKey::StringCipherWithStringKey()
   : key("")
{

}

StringCipherWithStringKey::~StringCipherWithStringKey()
{

}

void StringCipherWithStringKey::setKey(const KeyType &key)
{
   if (key.empty())
   {
      throw EmptyKey("Your key is empty.");
   }

   const char c = badAlphaFound(key);
   if (c != 0)
   {
      throw BadChar("Your key contains at least one character that is not in your alphabet.", c);
   }

   this->key = key;
}

