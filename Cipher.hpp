/*
 * Interface représentant tous les types de ciphers.
 */
#ifndef CIPHER_H
#define CIPHER_H

template <class FuncType, class ParamType>
class Cipher
{
public:
   virtual ~Cipher() {}

   virtual FuncType encode(const ParamType &) = 0;
   virtual FuncType decode(const ParamType &) = 0;
};

#endif // CIPHER_H
