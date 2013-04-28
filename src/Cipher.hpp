/*
 * Interface représentant tous les types de ciphers.
 */
#ifndef CIPHER_H
#define CIPHER_H

template <class ReturnType, class ParamType = ReturnType>
class Cipher
{
public:
   typedef ReturnType ReturnTypeT;
   typedef ParamType ParamTypeT;

   virtual ~Cipher() {}

   virtual const ReturnType encode(const ParamType &) = 0;
   virtual const ReturnType decode(const ParamType &) = 0;
};

#endif // CIPHER_H

