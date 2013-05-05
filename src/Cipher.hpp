/*
 * Interface for all cipher types.
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
   virtual const ParamType decode(const ReturnType &) = 0;
};

#endif