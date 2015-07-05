/*
 * Interface for all cipher types : Symmetric and asymmetric ciphers.
 */
#ifndef CIPHER_H
#define CIPHER_H

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

#endif