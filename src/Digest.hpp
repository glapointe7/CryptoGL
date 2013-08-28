
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

namespace Digest
{
   typedef std::vector<uint8_t> BytesContainer;
   
   /* Get the hexadecimal string from a vector of bytes. */
   const std::string hexDigest(const BytesContainer &bytes)
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
   
   const BytesContainer getBytesFromString(const std::string &str)
   {
      return BytesContainer(str.begin(), str.end());
   }
   
   const std::string getStringFromBytes(const BytesContainer &bytes)
   {
      return std::string(bytes.begin(), bytes.end());
   }
   
   const BytesContainer getBytesFromHexDigest(const std::string &hex_str)
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
   
   const std::string getStringFromBigInteger(const BigInteger &value)
   {
      return bigIntegerToString(value);
   }
   
   const std::string bigIntVectorToString(const std::vector<BigInteger> &V)
   {
      std::string str;
      for(const auto big_value : V)
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
      for(const auto str_value : tokens)
      {
         V.push_back(stringToBigInteger(str_value));
      }

      return V;
   }
}

#endif