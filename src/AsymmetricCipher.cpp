#include "AsymmetricCipher.hpp"

// Transforme le vecteur d'entiers en une string d'entiers.
const std::string AsymmetricCipher::dwordToString(const UInt64Container &numbers)
{
   std::string str;
   str.reserve(3 * numbers.size());
   
   for(const auto number : numbers)
   {
      str += std::to_string(number);
   }
   
   return str;
}