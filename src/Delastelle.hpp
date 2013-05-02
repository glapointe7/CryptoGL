
#ifndef DELASTELLE_HPP
#define	DELASTELLE_HPP

#include "SquareCipher.hpp"
#include <string>

class Delastelle : public SquareCipher
{
public:
   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;
   
   void setBlockLength(const unsigned int block_len);
  
private:
   unsigned int block_len;
};

#endif