
#ifndef DIGEST_HPP
#define DIGEST_HPP

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip> 
#include <iterator>

#include "big_integers/BigIntegerUtils.hh"
#include "big_integers/BigInteger.hh"
#include "Types.hpp"

namespace Digest
{
   /* Get the hexadecimal string from a vector of bytes. */
   template <class DataTypeVector>
   std::string hexDigest(const DataTypeVector &bytes)
   {
      const uint8_t data_size = sizeof(typename DataTypeVector::value_type) << 1;
      std::ostringstream ss;
      ss << std::hex << std::uppercase << std::setfill('0');

      for (const auto uint : bytes)
      {
         ss << std::setw(data_size) << uint;
      }

      return ss.str();
   }
   
   template<>
   std::string hexDigest(const BytesVector &bytes)
   {
      const std::string hex_digits = "0123456789ABCDEF";
      std::string hex_digest;
      hex_digest.reserve(bytes.size() << 1);
      
      for(const uint8_t byte : bytes)
      {
         hex_digest.append({hex_digits[(byte >> 4) & 0xF], hex_digits[byte & 0xF]});
      }
      
      return hex_digest;
   }

   BytesVector getBytesFromString(const std::string &str)
   {
      return BytesVector(str.begin(), str.end());
   }

   std::string getStringFromBytes(const BytesVector &bytes)
   {
      return std::string(bytes.begin(), bytes.end());
   }

   BytesVector getBytesFromHexDigest(const std::string &hex_str)
   {
      const uint64_t hex_len = hex_str.length();
      BytesVector bytes;
      bytes.reserve(hex_len >> 1);

      for (uint64_t i = 0; i < hex_len; i += 2)
      {
         const std::string hexa = {hex_str[i], hex_str[i+1]};
         bytes.push_back(strtoul(hexa.c_str(), nullptr, 16));
      }

      return bytes;
   }

   std::string getStringFromBigInteger(const BigInteger &value)
   {
      return bigIntegerToString(value);
   }

   std::string bigIntVectorToString(const std::vector<BigInteger> &V)
   {
      std::string str;
      for (const auto big_value : V)
      {
         str.append(getStringFromBigInteger(big_value));
         str.push_back(' ');
      }
      str.pop_back();

      return str;
   }

   std::vector<BigInteger> stringToBigIntVector(const std::string &str)
   {
      std::istringstream iss(str);
      std::vector<std::string> tokens;
      std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
              std::back_inserter<std::vector<std::string>>(tokens));

      std::vector<BigInteger> V;
      V.reserve(tokens.size());
      for (const auto str_value : tokens)
      {
         V.push_back(stringToBigInteger(str_value));
      }

      return V;
   }
}

#endif