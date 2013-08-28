
#ifndef HILL_HPP
#define HILL_HPP

#include "StringCipher.hpp"
#include "SquareMatrix.hpp"

#include <vector>

class Hill : public StringCipher
{   
public:
   typedef SquareMatrix::Matrix Matrix;
   
   explicit Hill(const Matrix &key) { this->key = new SquareMatrix(); setKey(key); }
   ~Hill() { delete key; }
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setKey(const Matrix &key);
   
private:
   const ClassicalType process(const ClassicalType &data, const SquareMatrix *K);
   
   SquareMatrix *key;
};

#endif