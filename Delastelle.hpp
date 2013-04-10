
#ifndef DELASTELLE_HPP
#define	DELASTELLE_HPP

#include "SquareCipher.h"
#include <string>

class Delastelle : public SquareCipher
{
public:
   Delastelle();
   
   virtual std::string encode(const std::string &);
   virtual std::string decode(const std::string &);
   
   void setBlockLength(const unsigned int block_len);
  
private:
   unsigned int block_len;
};

#endif	/* DELASTELLE_HPP */

