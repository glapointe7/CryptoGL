#include "TripleDES.hpp"

#include "DES.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "exceptions/EmptyKey.hpp"

void TripleDES::setKey(const BytesContainer &key)
{
   if(key.size() != 8)
   {
      throw BadKeyLength("The key must be 8 bytes length.", key.size());
   }
   
   this->key = key;
}

void TripleDES::setSecondKey(const BytesContainer &key2)
{
   if(key2.size() != 8)
   {
      throw BadKeyLength("The key must be 8 bytes length.", key2.size());
   }
   
   this->key2 = key2;
}

void TripleDES::setThirdKey(const BytesContainer &key3)
{
   if(key3.size() != 8)
   {
      throw BadKeyLength("The key must be 8 bytes length.", key3.size());
   }
   
   this->key3 = key3;
}

const TripleDES::BytesContainer TripleDES::encode(const BytesContainer &clear_text)
{
   DES *D = new DES();
   D->setKey(key);
   const BytesContainer first = D->encode(clear_text);
   
   if(key3.empty())
   {
      key3 = key;
   }
  
   D->setKey(key2);
   const BytesContainer second = D->decode(first);
   
   D->setKey(key3);
   return D->encode(second);
}

const TripleDES::BytesContainer TripleDES::decode(const BytesContainer &cipher_text)
{
   DES *D = new DES();
   
   if(key3.empty())
   {
      key3 = key;
   }
   
   D->setKey(key3);
   const BytesContainer first = D->decode(cipher_text);
   
   D->setKey(key2);
   const BytesContainer second = D->encode(first);
   
   D->setKey(key);
   return D->decode(second);
}