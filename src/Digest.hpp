
#ifndef DIGEST_HPP
#define DIGEST_HPP

#include <sstream>
#include <algorithm>
#include <iterator>

#include "big_integers/BigIntegerUtils.hh"
#include "big_integers/BigInteger.hh"
#include "Types.hpp"

namespace Digest
{
    String getStringFromBigInteger(const BigInteger &value)
    {
        return bigIntegerToString(value);
    }
    
    String bigIntVectorToString(const std::vector<BigInteger> &V)
    {
        String str;
        for (const auto &big_value : V)
        {
            str.append(getStringFromBigInteger(big_value));
            str.push_back(' ');
        }
        str.pop_back();

        return str;
    }
    
    Vector<BigInteger> stringToBigIntVector(const String &str)
    {
        std::istringstream iss(str);
        Vector<String> tokens;
        std::copy(std::istream_iterator<String>(iss), std::istream_iterator<String>(),
                std::back_inserter<Vector < String >> (tokens));

        Vector<BigInteger> V;
        V.reserve(tokens.size());
        for (const auto &str_value : tokens)
        {
            V.push_back(stringToBigInteger(str_value));
        }

        return V;
    }
}

#endif