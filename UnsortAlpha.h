
#ifndef UNSORTALPHA_H
#define UNSORTALPHA_H

#include "StringCipher.h"

#include <string>

class UnsortAlpha : public StringCipher
{
public:
   UnsortAlpha();
   
   std::string encode(const std::string &);
   std::string decode(const std::string &);

   void setVerticalAlpha();
   void setHorizontalAlpha();
   void setKey(const std::string key);

private:
   std::string key;
   std::string unsort_alpha;
};

#endif // UNSORTALPHA_H
