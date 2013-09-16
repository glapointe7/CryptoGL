
#ifndef STRINGCIPHERWITHPERMUTATIONKEY_HPP
#define STRINGCIPHERWITHPERMUTATIONKEY_HPP

#include "StringCipher.hpp"

#include "exceptions/BadKey.hpp"

#include <vector>

class StringCipherWithPermutationKey : public StringCipher
{   
public:
   using KeyType = Int32Vector;

   const KeyType getKey() const { return key; }
   void setKey(const KeyType &key);

protected:
   StringCipherWithPermutationKey() {}
   virtual ~StringCipherWithPermutationKey() {}
   
   KeyType key;
   
private:
   /* Check if the key contains unique integers. */
   static bool isUniqueWithoutMissingIntegers(const KeyType &key);
      
   using BadPermutationKey = BadKey;
};

#endif