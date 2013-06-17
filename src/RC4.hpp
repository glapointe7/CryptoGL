#ifndef RC4_HPP
#define	RC4_HPP

#include "StreamCipher.hpp"

class RC4 : public StreamCipher
{
public:
   explicit RC4(const BytesContainer &key);
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
private:
   virtual void keySetup() final;
   void initialize();
   
   BytesContainer state;
};

#endif