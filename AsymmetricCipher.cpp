#include "AsymmetricCipher.hpp"

AsymmetricCipher::Numbers 
AsymmetricCipher::getPublicKey() const
{
   return public_key;
}

// Transforme les caractères en hexadécimal et renvoie la chaîne d'hexa.
std::string AsymmetricCipher::hexDigest(const Bytes_Container &bytes) const
{
   std::string str("");
   str.reserve(3 * bytes.size());
   
   for(auto c : bytes)
   {
      char buffer[3];
      sprintf(&buffer, "%02X ", c);
      str += *buffer;
   }
   
   return str;
}

// Transforme le vecteur d'entiers en une string d'entiers.
std::string AsymmetricCipher::toString(const Numbers &numbers) const
{
   std::string str("");
   str.reserve(3 * numbers.size());
   
   for(auto number : numbers)
   {
      str += std::to_string(number);
   }
   
   return str;
}

AsymmetricCipher::Bytes_Container 
AsymmetricCipher::toBytes(const std::string &text)
{
   return Bytes_Container(text.begin(), text.end());
}