#ifndef RC4_HPP
#define	RC4_HPP

#include "StreamCipher.hpp"

class RC4 : public StreamCipher
{
public:
   
   virtual const BytesContainer encode(const BytesContainer &) final;
   virtual const BytesContainer decode(const BytesContainer &) final;
   
   virtual void setKey(const BytesContainer &) final;
   
private:
   virtual void keySetup() final;
   void initialize();
   
   BytesContainer state;
};

#endif