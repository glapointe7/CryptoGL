
#ifndef DELASTELLE_HPP
#define DELASTELLE_HPP

#include "SquareCipher.hpp"
#include <string>

#include "exceptions/Exception.hpp"

class Delastelle : public SquareCipher
{
public:
   Delastelle(const KeyType &key, const uint32_t block_length)
      : SquareCipher(key), block_len(block_length) {}
   
   explicit Delastelle(const KeyType &key) : Delastelle(key, 5) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setBlockLength(const uint32_t block_len);
  
private:
   using ZeroBlockLength = Exception;
   
   uint32_t block_len;
};

#endif