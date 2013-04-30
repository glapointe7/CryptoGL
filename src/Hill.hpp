
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.hpp"
#include "Matrix.hpp"

#include <vector>

class Hill : public StringCipher
{
public:
   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;
   
   void setKey(const Matrix &);
   
private:
   Matrix key;
};

#endif