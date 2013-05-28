
#ifndef COLLON_HPP
#define COLLON_HPP

#include "SquareCipher.hpp"

#include <string>

class Collon : public SquareCipher
{
public:
   Collon(const KeyType &key) : SquareCipher(key) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

   void setBlockLength(const uint32_t series);

private:
   uint32_t block_len;
};
#endif