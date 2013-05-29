
#ifndef COLLON_HPP
#define COLLON_HPP

#include "SquareCipher.hpp"

#include <string>

#include "exceptions/Exception.hpp"

class Collon : public SquareCipher
{
public:
   explicit Collon(const KeyType &key) : SquareCipher(key) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

   void setBlockLength(const uint32_t block_len);

private:
   using ZeroBlockLength = Exception;
   
   uint32_t block_len = 5;
};
#endif