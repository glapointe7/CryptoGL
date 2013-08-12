#include "AsymmetricCipher.hpp"

// Transform a integers vector to a string of integers.
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