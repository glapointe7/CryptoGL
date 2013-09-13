
#ifndef DIGEST_HPP
#define DIGEST_HPP

#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "big_integers/BigIntegerUtils.hh"
#include "big_integers/BigInteger.hh"
#include "Types.hpp"

namespace Digest
{
   /* Get the hexadecimal string from a vector of bytes. */
   template <class DataTypeVector>
   const std::string hexDigest(const DataTypeVector &bytes)
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
   const std::string hexDigest(const BytesVector &bytes)
   {
      const int8_t hex_digits[] = "0123456789ABCDEF";
      std::string hex_digest;
      hex_digest.reserve(bytes.size() << 1);
      
      for(const uint8_t byte : bytes)
      {
         hex_digest.append({hex_digits[(byte >> 4) & 0xF], hex_digits[byte & 0xF]});
      }
      
      return hex_digest;
   }

   const BytesVector getBytesFromString(const std::string &str)
   {
      return BytesVector(str.begin(), str.end());
   }

   const std::string getStringFromBytes(const BytesVector &bytes)
   {
      return std::string(bytes.begin(), bytes.end());
   }

   const BytesVector getBytesFromHexDigest(const std::string &hex_str)
   {
      const uint64_t hex_len = hex_str.length();
      BytesVector bytes;
      bytes.reserve(hex_len >> 1);

      for (uint64_t i = 0; i < hex_len; i += 2)
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

   const std::string getStringFromBigInteger(const BigInteger &value)
   {
      return bigIntegerToString(value);
   }

   const std::string bigIntVectorToString(const std::vector<BigInteger> &V)
   {
      std::string str;
      for (const auto big_value : V)
      {
         str += getStringFromBigInteger(big_value);
         str.push_back(' ');
      }
      str.pop_back();

      return str;
   }

   const std::vector<BigInteger> stringToBigIntVector(const std::string &str)
   {
      std::istringstream iss(str);
      std::vector<std::string> tokens;
      std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
              std::back_inserter<std::vector<std::string> >(tokens));

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