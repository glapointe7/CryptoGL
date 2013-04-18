
#ifndef DELASTELLE_HPP
#define	DELASTELLE_HPP

#include "SquareCipher.hpp"
#include <string>

class Delastelle : public SquareCipher
{
public:
   Delastelle();
   
   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);
   
   void setBlockLength(const unsigned int block_len);
  
private:
   unsigned int block_len;
};

#endif