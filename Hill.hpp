
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.h"
#include <vector>

class Hill : public StringCipher
{
public:
   Hill();
   
   std::string encode(const std::string &);
   std::string decode(const std::string &);
   
   void setKey(const std::vector<std::vector<long> > &);
   
private:
   std::vector< std::vector<long> > key;
};

#endif	/* HILL_HPP */

