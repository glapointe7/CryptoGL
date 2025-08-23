#pragma once

#include <sstream>
#include <algorithm>
#include <iterator>

#include "../../big_integers/BigIntegerUtils.hh"
#include "../../big_integers/BigInteger.hh"
#include "../types/Types.hpp"

namespace Digest
{
    String getStringFromBigInteger(const BigInteger &value)
    {
        return String(bigIntegerToString(value));
    }
    
    String bigIntVectorToString(const Vector<BigInteger> &V)
    {
        String str;
        for (const auto &big_value : V)
        {
            str.append(getStringFromBigInteger(big_value));
            str.push_back(' ');
        }
        str.pop_back();

        return String(str);
    }
    
    Vector<BigInteger> stringToBigIntVector(const String &str)
    {
        std::istringstream iss(str.toStdString());
        Vector<std::string> tokens;
        std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
                std::back_inserter<Vector < std::string >> (tokens));

        Vector<BigInteger> V;
        V.reserve(tokens.size());
        for (const std::string &str_value : tokens)
        {
            V.push_back(stringToBigInteger(str_value));
        }

        return V;
    }
}