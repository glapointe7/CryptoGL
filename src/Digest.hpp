
#ifndef DIGEST_HPP
#define DIGEST_HPP

#include <sstream>
#include <algorithm>
#include <iterator>

#include "big_integers/BigIntegerUtils.hh"
#include "big_integers/BigInteger.hh"
#include "Types.hpp"
#include "String.hpp"

namespace Digest
{
   std::string getStringFromBigInteger(const BigInteger &value)
   {
      return bigIntegerToString(value);
   }

   std::string bigIntVectorToString(const std::vector<BigInteger> &V)
   {
      std::string str;
      for (const auto &big_value : V)
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
      for (const auto &str_value : tokens)
      {
         V.push_back(stringToBigInteger(str_value));
      }

      return V;
   }
}

#endif