#include "TripleDES.hpp"

#include "DES.hpp"
#include "BlockCipherOperationModes.hpp"

#include "exceptions/BadKeyLength.hpp"

TripleDES::TripleDES(const BytesContainer &key1, const BytesContainer &key2, const BytesContainer &key3)
{
   checkKey(key1);
   this->key = key1;

   checkKey(key2);
   this->key2 = key2;

   checkKey(key3);
   this->key3 = key3;
}

void TripleDES::checkKey(const BytesContainer &key)
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

const TripleDES::BytesContainer TripleDES::encode(const BytesContainer &clear_text)
{
   DES *D1 = new DES(key, mode);
   const BytesContainer first = D1->encode(clear_text);
   delete D1;
   
   DES *D2 = new DES(key2, mode);
   const BytesContainer second = D2->decode(first);
   delete D2;
   
   DES *D3 = new DES(key3, mode);
   return D3->encode(second);
}

const TripleDES::BytesContainer TripleDES::decode(const BytesContainer &cipher_text)
{
   DES *D1 = new DES(key3, mode);
   const BytesContainer first = D1->decode(cipher_text);
   delete D1;
   
   DES *D2 = new DES(key2, mode);
   const BytesContainer second = D2->encode(first);
   delete D2;

   DES *D3 = new DES(key, mode);
   return D3->decode(second);
}