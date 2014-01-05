
#ifndef HILL_HPP
#define HILL_HPP

#include "StringCipher.hpp"
#include "SquareMatrix.hpp"

#include <vector>

class Hill : public StringCipher
{   
public:   
   explicit Hill(const Int32Matrix &key) { setKey(key); }
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;
   
   void setKey(const Int32Matrix &key);
   
private:
   ClassicalType process(const ClassicalType &data, const SquareMatrix K) const;
   
   SquareMatrix key;
};

#endif