
#include "HashFunction.hpp"

#include <sstream>

// Transforme les octets en nombres hexadécimaux sous forme d'une string.
const std::string HashFunction::hexDigest(const BytesContainer &bytes)
{
   std::stringstream ss;

   for (const auto byte : bytes)
   {
      ss.setf(std::ios::hex, std::ios::basefield);
      ss.fill('0');
      ss.width(2);
      ss << static_cast<uint16_t>(byte);
   }
   
   return ss.str();
}

const HashFunction::BytesContainer 
HashFunction::getBytesFromString(const std::string &str)
{  
   return BytesContainer(str.begin(), str.end());
}

const std::string HashFunction::getStringFromBytes(const BytesContainer &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}

// Retourne un vecteur de bits à partir d'un vecteur d'octets en LITTLE ENDIAN.
const HashFunction::BitsContainer HashFunction::getBitsFromData(const BytesContainer &data)
{
   BitsContainer bits;
   bits.reserve(data.size() * 8);
   
   for(auto byte : data)
   {
      for(char i = 7; i >= 0; --i)
      {
         bits.push_back((byte >> i) & 0x1);
      }
   }
   
   return bits;
}
