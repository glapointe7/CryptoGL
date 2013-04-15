
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.h"

#include <vector>

class Hill : public StringCipher
{
public:
   typedef std::vector<std::vector<long>> Matrix;

   Hill();
   
   std::string encode(const std::string &);
   std::string decode(const std::string &);
   
   void setKey(const Matrix &);
   
private:
   Matrix key;
};

#endif	/* HILL_HPP */

