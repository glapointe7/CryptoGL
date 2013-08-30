/*
 * Interface for all cipher types : Symmetric and asymmetric ciphers.
 */
#ifndef CIPHER_H
#define CIPHER_H

#include "Types.hpp"

template <class ReturnType, class ParamType = ReturnType>
class Cipher
{
protected:
   virtual ~Cipher() {}
   virtual const ReturnType encode(const ParamType &) = 0;
   virtual const ParamType decode(const ReturnType &) = 0;
};

#endif