
#ifndef COLLON_HPP
#define COLLON_HPP

#include "SquareCipher.hpp"

#include <string>

//#include "exceptions/Exception.hpp"

class Collon : public SquareCipher
{
public:
   Collon(const KeyType &key, const ClassicalType &alpha, const uint32_t block_length)
      : SquareCipher(key, alpha), block_len(block_length) {}
      
   Collon(const KeyType &key, const uint32_t block_length)
      : Collon(key, ClassicalType::grid_uppercase_fr, block_length) {}
   
   explicit Collon(const KeyType &key) 
      : Collon(key, 5) {}
   
   ClassicalType encode(const ClassicalType &clear_text) override;
   ClassicalType decode(const ClassicalType &cipher_text) override;

private:
   //using ZeroBlockLength = Exception;
   
   const uint32_t block_len;
};
#endif