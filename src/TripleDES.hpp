
#ifndef TRIPLEDES_HPP
#define	TRIPLEDES_HPP

#include "SymmetricCipher.hpp"
#include "BlockCipherOperationModes.hpp"

class TripleDES : public SymmetricCipher
{
public:
   virtual const BytesContainer encode(const BytesContainer &) final;
   virtual const BytesContainer decode(const BytesContainer &) final;

   virtual void setKey(const BytesContainer &key) final;
   void setSecondKey(const BytesContainer &key2);
   void setThirdKey(const BytesContainer &key3);
   void setOperationMode(const OperationModes mode);
   
private:
   BytesContainer key2, key3;
   OperationModes mode = OperationModes::ECB;
};

#endif