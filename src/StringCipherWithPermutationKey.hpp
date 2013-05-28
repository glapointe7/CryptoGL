
#ifndef STRINGCIPHERWITHPERMUTATIONKEY_HPP
#define	STRINGCIPHERWITHPERMUTATIONKEY_HPP

#include "StringCipher.hpp"

#include "exceptions/BadKey.hpp"

#include <vector>

class StringCipherWithPermutationKey : public StringCipher
{   
public:
   typedef std::vector<int32_t> KeyType;

   StringCipherWithPermutationKey() {}
   virtual ~StringCipherWithPermutationKey() {}

   const KeyType getKey() const { return key; }
   virtual void setKey(const KeyType &key);

protected:
   /* Check if the key contains unique integers. */
   static bool isUniqueWithoutMissingIntegers(const KeyType &key);
   
   KeyType key;
   
   using BadPermutationKey = BadKey;
};

#endif