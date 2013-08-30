#ifndef RSA_HPP
#define RSA_HPP

#include "AsymmetricCipher.hpp"

class RSA : public AsymmetricCipher
{
public:
   RSA(const BigInteger &p, const BigInteger &q, const BigInteger &e);
   
private:
   /* The Euler Totient function : phi(n) = (p-1)(q-1). */
   const BigInteger phi(const BigInteger &n) const;
   
   BigInteger n, e, d;
};

#endif