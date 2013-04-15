
#ifndef UNSORTALPHA_H
#define UNSORTALPHA_H

#include "StringCipher.h"

#include <string>

class UnsortAlpha : public StringCipher
{
public:
   UnsortAlpha();
   
   virtual const ClassicalType encode(const ClassicalType &);
   virtual const ClassicalType decode(const ClassicalType &);

   void setVerticalAlpha();
   void setHorizontalAlpha();
   void setKey(const std::string key);

private:
   std::string key;
   ClassicalType unsort_alpha;
};

#endif