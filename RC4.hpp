#ifndef RC4_HPP
#define	RC4_HPP

#include "SymmetricCipher.hpp"

class RC4 : public SymmetricCipher
{
public:
   
   virtual BytesContainer encode(const BytesContainer &);
   virtual BytesContainer decode(const BytesContainer &);
   
   virtual void setKey(const BytesContainer &);
   
private:
   void initKeySchedule();
   void initState();
   
   BytesContainer state;
};

#endif