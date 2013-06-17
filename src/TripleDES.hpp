
#ifndef TRIPLEDES_HPP
#define	TRIPLEDES_HPP

#include "SymmetricCipher.hpp"
#include "BlockCipherOperationModes.hpp"

class TripleDES : public SymmetricCipher
{
public:
   TripleDES(const BytesContainer &key1, const BytesContainer &key2, const BytesContainer &key3);
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;

   void setOperationMode(const OperationModes mode);
   
private:
   static void checkKey(const BytesContainer &key);
   
   BytesContainer key2, key3;
   OperationModes mode = OperationModes::ECB;
};

#endif