
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.hpp"
#include "Matrix.hpp"

#include <vector>

class Hill : public StringCipher
{
   typedef std::vector<std::vector<int32_t> > Matrice;
   
public:
   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;
   
   void setKey(const Matrice &);
   
private:
   const ClassicalType process(const ClassicalType &, const Matrix &);
   
   Matrix key;
};

#endif