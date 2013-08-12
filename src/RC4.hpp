#ifndef RC4_HPP
#define	RC4_HPP

#include "StreamCipher.hpp"

class RC4 : public StreamCipher
{
public:
   explicit RC4(const BytesContainer &key) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
   virtual void setKey(const BytesContainer &key) final;
   
private:
   virtual void keySetup() final;
   
   uint8_t state[255];
};

#endif