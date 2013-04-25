#include "HashFunctions.hpp"

#include <sstream>

// Transforme les octets en nombres hexadécimaux sous forme d'une string.
std::string HashFunctions::hexDigest(const BytesContainer &bytes)
{
   std::stringstream ss;

   for (auto byte : bytes)
   {
      ss << std::hex << (unsigned int)byte;
      ss << " ";
   }
   
   return ss.str();
}

HashFunctions::BytesContainer 
HashFunctions::getBytesFromString(const std::string &str)
{  
   return BytesContainer(str.begin(), str.end());
}

std::string HashFunctions::getStringFromBytes(const BytesContainer &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}

// Retourne un vecteur de bits à partir d'un vecteur d'octets en LITTLE ENDIAN.
HashFunctions::BitsContainer HashFunctions::getBitsFromData(const BytesContainer &data)
{
   BitsContainer bits;
   bits.reserve(data.size() << 3);
   
   for(auto byte : data)
   {
      for(char i = 7; i >= 0; --i)
      {
         bits.push_back((byte >> i) & 0x1);
      }
   }
   
   return bits;
}