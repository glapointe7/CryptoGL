
#include "HashFunction.hpp"

#include <sstream>

// Transforme les octets en nombres hexadécimaux sous forme d'une string.
const std::string HashFunction::hexDigest(const BytesContainer &bytes)
{
   std::ostringstream ss;
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

const HashFunction::BytesContainer 
HashFunction::getBytesFromString(const std::string &str)
{  
   return BytesContainer(str.begin(), str.end());
}

const std::string HashFunction::getStringFromBytes(const BytesContainer &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}

const HashFunction::BytesContainer
HashFunction::getBytesFromHexDigest(const std::string &hex_str)
{
   const uint32_t hex_len = hex_str.length();
   BytesContainer bytes;
   bytes.reserve(hex_len >> 1);

   for (uint32_t i = 0; i < hex_len; i += 2)
   {
      const std::string hexa = hex_str.substr(i, 2);
      std::istringstream ss(hexa);
      ss.setf(std::ios::hex, std::ios::basefield);
      uint16_t x;
      ss >> x;
      bytes.push_back(static_cast<uint8_t> (x));
   }

   return bytes;
}
