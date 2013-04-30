
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.hpp"

#include <vector>

class Hill : public StringCipher
{
public:
   typedef std::vector<std::vector<long> > Matrix;
   
   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;
   
   void setKey(const Matrix &);
   
private:
   Matrix key;
};

#endif