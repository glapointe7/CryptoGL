
#ifndef DELASTELLE_HPP
#define	DELASTELLE_HPP

#include "SquareCipher.hpp"
#include <string>

class Delastelle : public SquareCipher
{
public:
   Delastelle(const KeyType &key) : SquareCipher(key) {}
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setBlockLength(const uint32_t block_len);
  
private:
   uint32_t block_len;
};

#endif