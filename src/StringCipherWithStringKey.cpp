
#include "StringCipherWithStringKey.hpp"

#include "exceptions/EmptyKey.hpp"
#include "exceptions/BadChar.hpp"

void StringCipherWithStringKey::checkKey(const KeyType &key)
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
}

void StringCipherWithStringKey::setKey(const KeyType &key)
{
   checkKey(key);
   this->key = key;
}