#ifndef RC4_HPP
#define	RC4_HPP

#include "SymmetricCipher.hpp"

class RC4 : public SymmetricCipher
{
public:
   
   virtual BytesContainer encode(const BytesContainer &);
   virtual BytesContainer decode(const BytesContainer &);
   
   virtual void setKey(const BytesContainer &);
   void initKeySchedule();
   void initState();
   
private:
   BytesContainer state;
};

#endif