#ifndef RC4_HPP
#define RC4_HPP

#include "StreamCipher.hpp"

class RC4 : public StreamCipher
{
public:
   explicit RC4(const BytesVector &key) { setKey(key); }
   
   virtual const BytesVector encode(const BytesVector &clear_text) final;
   virtual const BytesVector decode(const BytesVector &cipher_text) final;
   
   virtual void setKey(const BytesVector &key) final;
   
private:
   virtual void keySetup() final;
   
   uint8_t subkeys[255];
};

#endif