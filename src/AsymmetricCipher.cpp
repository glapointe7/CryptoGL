#include "AsymmetricCipher.hpp"

#include <sstream>

// Transforme les caractères en hexadécimal et renvoie la chaîne d'hexa.
const std::string AsymmetricCipher::hexDigest(const BytesContainer &bytes)
{
   std::stringstream ss;
   ss.setf(std::ios::hex, std::ios::basefield);
   ss << std::uppercase;

   for (const auto byte : bytes)
   {
      ss.fill('0');
      ss.width(2);
      ss << static_cast<uint16_t> (byte);
   }

   return ss.str();
}

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

// Transforme le vecteur d'entiers en une string d'entiers.
const std::string AsymmetricCipher::byteToString(const BytesContainer &numbers)
{
   std::string str;
   str.reserve(3 * numbers.size());
   
   for(const auto number : numbers)
   {
      str += number;
   }
   
   return str;
}

const AsymmetricCipher::BytesContainer 
AsymmetricCipher::stringToBytes(const std::string &text)
{
   return BytesContainer(text.begin(), text.end());
}