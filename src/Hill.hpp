
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.hpp"
#include "Matrix.hpp"

#include <vector>

class Hill : public StringCipher
{
   typedef std::vector<std::vector<int32_t> > Matrice;
   
public:
   explicit Hill(const Matrice &key) { this->key = new Matrix(); setKey(key); }
   ~Hill() { delete key; }
   
   virtual const ClassicalType encode(const ClassicalType &clear_text) final;
   virtual const ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setKey(const Matrice &key);
   
private:
   const ClassicalType process(const ClassicalType &data, const Matrix *K);
   
   Matrix *key;
};

#endif