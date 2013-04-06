
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.h"
#include "Matrix.hpp"
#include <vector>

class Hill : public StringCipher
{
public:
   Hill();
   
   std::string encode(const std::string &);
   std::string decode(const std::string &);
   
   void setKey(const Matrix &);
   
private:
   Matrix key;
};

#endif	/* HILL_HPP */

