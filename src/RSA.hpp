#pragma once

#include "AsymmetricCipher.hpp"

namespace CryptoGL
{
    class RSA : public AsymmetricCipher
    {
    public:
        RSA(const BigInteger &p, const BigInteger &q, const BigInteger &e);

    private:
        /* The Euler Totient function : phi(n) = (p-1)(q-1). */
        BigInteger phi(const BigInteger &n) const;

        BigInteger n, e, d;
    };
}