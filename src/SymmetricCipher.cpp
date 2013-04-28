#include "SymmetricCipher.hpp"

#include <sstream>

void SymmetricCipher::save(const std::string &filename, const BytesContainer &data)
{

}

SymmetricCipher::BytesContainer SymmetricCipher::load(const std::string &filename)
{
   BytesContainer bytes;

   return bytes;
}

// Transforme les octets en nombres hexadécimaux sous forme d'une string.
std::string SymmetricCipher::hexDigest(const BytesContainer &bytes)
{
   std::stringstream ss;

   for (auto byte : bytes)
   {
      ss << std::hex << (unsigned int)byte;
      ss << " ";
   }
   
   return ss.str();
}

const SymmetricCipher::BytesContainer 
SymmetricCipher::getBytesFromString(const std::string &str)
{  
   return BytesContainer(str.begin(), str.end());
}

const std::string SymmetricCipher::getStringFromBytes(const BytesContainer &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}