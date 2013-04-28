#ifndef RC4_HPP
#define	RC4_HPP

#include "StreamCipher.hpp"

class RC4 : public StreamCipher
{
public:
   
   virtual const BytesContainer encode(const BytesContainer &);
   virtual const BytesContainer decode(const BytesContainer &);
   
   virtual void setKey(const BytesContainer &);
   
private:
   void initKeySchedule();
   void initialize();
   
   BytesContainer state;
};

#endif