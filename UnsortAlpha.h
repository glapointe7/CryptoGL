#ifndef UNSORTALPHA_H
#define UNSORTALPHA_H

#include "StringCipher.h"
#include <vector>

class UnsortAlpha : public StringCipher
{
public:
   UnsortAlpha();
   ~UnsortAlpha();

   void setVerticalAlpha();
   void setHorizontalAlpha();
   void setKey(const string key);

protected:
   string encode();
   string decode();

private:
   string key;
   string unsort_alpha;
};
#endif