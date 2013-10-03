
#ifndef TRIPLEDES_HPP
#define TRIPLEDES_HPP

#include "SymmetricCipher.hpp"
#include "BlockCipherModes.hpp"

class TripleDES : public SymmetricCipher
{
public:
   TripleDES(const BytesVector &key1, const BytesVector &key2, const BytesVector &key3);
   
   virtual const BytesVector encode(const BytesVector &clear_text) final;
   virtual const BytesVector decode(const BytesVector &cipher_text) final;
   
   virtual void setKey(const BytesVector &key) final;

   void setOperationMode(const OperationModes mode);
   
private:   
   BytesVector key2, key3;
   OperationModes mode = OperationModes::ECB;
};

#endif