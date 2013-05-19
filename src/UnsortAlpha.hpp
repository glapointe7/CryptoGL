
#ifndef UNSORTALPHA_HPP
#define UNSORTALPHA_HPP

#include "StringCipher.hpp"

#include <string>

class UnsortAlpha : public StringCipher
{
public:
   UnsortAlpha();
   
   virtual const ClassicalType encode(const ClassicalType &clear_text);
   virtual const ClassicalType decode(const ClassicalType &cipher_text);

   void setVerticalAlpha();
   void setHorizontalAlpha();
   
   void setKey(const std::string &key);

private:
   ClassicalType unsort_alpha;
   std::string key;
};

#endif