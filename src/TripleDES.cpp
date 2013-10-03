#include "TripleDES.hpp"

#include "DES.hpp"

#include "exceptions/BadKeyLength.hpp"

TripleDES::TripleDES(const BytesVector &key1, const BytesVector &key2, const BytesVector &key3)
{
   setKey(key1);
   this->key = key1;

   setKey(key2);
   this->key2 = key2;

   setKey(key3);
   this->key3 = key3;
}

void TripleDES::setKey(const BytesVector &key)
{
   if (key.size() != 8)
   {
      throw BadKeyLength("The key must be 8 bytes length.", key.size());
   }
}

void TripleDES::setOperationMode(const OperationModes mode)
{
   this->mode = mode;
}

const BytesVector TripleDES::encode(const BytesVector &clear_text)
{
   DES *D1 = new DES(key, mode);
   const BytesVector first = D1->encode(clear_text);
   delete D1;
   
   DES *D2 = new DES(key2, mode);
   const BytesVector second = D2->decode(first);
   delete D2;
   
   DES *D3 = new DES(key3, mode);
   return D3->encode(second);
}

const BytesVector TripleDES::decode(const BytesVector &cipher_text)
{
   DES *D1 = new DES(key3, mode);
   const BytesVector first = D1->decode(cipher_text);
   delete D1;
   
   DES *D2 = new DES(key2, mode);
   const BytesVector second = D2->encode(first);
   delete D2;

   DES *D3 = new DES(key, mode);
   return D3->decode(second);
}