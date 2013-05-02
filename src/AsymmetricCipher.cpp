#include "AsymmetricCipher.hpp"

#include <sstream>

AsymmetricCipher::UInt64Container 
AsymmetricCipher::getPublicKey() const
{
   return public_key;
}

// Transforme les caractères en hexadécimal et renvoie la chaîne d'hexa.
std::string AsymmetricCipher::hexDigest(const BytesContainer &bytes)
{
   std::stringstream ss;

   for (auto byte : bytes)
   {
      ss << std::hex << static_cast<uint32_t>(byte);
      ss << " ";
   }
   
   return ss.str();
}

// Transforme le vecteur d'entiers en une string d'entiers.
std::string AsymmetricCipher::toString(const UInt64Container &numbers)
{
   std::string str("");
   str.reserve(3 * numbers.size());
   
   for(auto number : numbers)
   {
      str += std::to_string(number);
   }
   
   return str;
}

AsymmetricCipher::BytesContainer 
AsymmetricCipher::toBytes(const std::string &text)
{
   return BytesContainer(text.begin(), text.end());
}