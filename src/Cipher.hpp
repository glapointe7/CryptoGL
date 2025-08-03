/*
 * Interface for all cipher types : Symmetric and asymmetric ciphers.
 */
#pragma once

#include "Types.hpp"

namespace CryptoGL
{
    template <class ReturnType, class ParamType = ReturnType>
    class Cipher
    {
    protected:
        virtual ~Cipher() { }
        virtual ReturnType encode(const ParamType &) = 0;
        virtual ParamType decode(const ReturnType &) = 0;
    };
}