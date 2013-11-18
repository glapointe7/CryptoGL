
#ifndef UNSORTALPHA_HPP
#define UNSORTALPHA_HPP

#include "StringCipherWithStringKey.hpp"

#include <string>
#include <functional>

class UnsortAlpha : public StringCipherWithStringKey
{
public:
   explicit UnsortAlpha(const KeyType &key);
   
   virtual ClassicalType encode(const ClassicalType &clear_text) final;
   virtual ClassicalType decode(const ClassicalType &cipher_text) final;

   /* Unordered the alphabet in the vertical way. */
   void setVerticalAlpha();
   
   /* Unordered the alphabet in the horizontal way. */
   void setHorizontalAlpha();

private:
   typedef std::function<char(const ClassicalType &, const ClassicalType &, const char)> GetCharFunction;
   
   ClassicalType unsort_alpha;
   const GetCharFunction charEncode, charDecode;
   
   static char sortAlpha(const ClassicalType &alpha, const ClassicalType &unsort_alpha, const char c)
   {
      return unsort_alpha[alpha.find(c)];
   }

   static char unorderedAlpha(const ClassicalType &alpha, const ClassicalType &unsort_alpha, const char c)
   {
      return alpha[unsort_alpha.find(c)];
   }
   
   ClassicalType process(const ClassicalType &text, const GetCharFunction &getNextChar);
};

#endif