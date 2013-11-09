
#ifndef COLLON_HPP
#define COLLON_HPP

#include "SquareCipher.hpp"

#include <string>

//#include "exceptions/Exception.hpp"

class Collon : public SquareCipher
{
public:
   Collon(const KeyType &key, const uint32_t block_length)
      : SquareCipher(key), block_len(block_length) {}
   
   explicit Collon(const KeyType &key) 
      : Collon(key, 5) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;

   //void setBlockLength(const uint32_t block_len);

private:
   //using ZeroBlockLength = Exception;
   
   const uint32_t block_len;
};
#endif