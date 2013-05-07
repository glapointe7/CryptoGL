
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.hpp"
#include "Matrix.hpp"

#include <vector>

class Hill : public StringCipher
{
   typedef std::vector<std::vector<int32_t> > Matrice;
   
public:
   Hill() {key = new Matrix(); }
   ~Hill() { delete key; }
   
   const ClassicalType encode(const ClassicalType &) final;
   const ClassicalType decode(const ClassicalType &) final;
   
   void setKey(const Matrice &);
   
private:
   const ClassicalType process(const ClassicalType &data, const Matrix *K);
   
   Matrix *key;
};

#endif