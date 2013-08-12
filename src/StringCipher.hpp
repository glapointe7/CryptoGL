
#ifndef STRINGCIPHER_HPP
#define STRINGCIPHER_HPP

#include "Cipher.hpp"

#include "String.hpp"

#include <string>

class StringCipher : public Cipher<std::string>
{
public:
   typedef std::string ClassicalType; 
   
   void eraseBadCharacters(ClassicalType &text) const;
   
   virtual void setAlpha(const ClassicalType &alpha);
   const ClassicalType getAlpha() const;

protected: 
   StringCipher() : alpha(String::uppercase) {}
   virtual ~StringCipher() {}

   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;
   
   const ClassicalType appendChars(const ClassicalType &data, const uint32_t to_add, const char c);
   const char badAlphaFound(const ClassicalType &text) const;
   void checkAlpha(const ClassicalType &alpha) const;
   
   ClassicalType alpha;
};

#endif
