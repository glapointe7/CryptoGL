/*
 * 
 */
#ifndef STRINGCIPHER_HPP
#define STRINGCIPHER_HPP

#include "Cipher.hpp"

#include "String.hpp"

#include <string>

class StringCipher : public Cipher<std::string>
{
public:
   typedef std::string ClassicalType; 
   
   StringCipher() : alpha(String::uppercase) {}
   virtual ~StringCipher() {}

   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;
   
   void save(const std::string &, const ClassicalType &);
   const ClassicalType load(const std::string &);
   
   void eraseBadCharacters(ClassicalType &) const;
   
   void setAlpha(const ClassicalType &);
   const ClassicalType getAlpha() const;

protected: 
   const ClassicalType appendChars(const ClassicalType &, const uint32_t, const char);
   const char badAlphaFound(const ClassicalType &) const;
   
   ClassicalType alpha;
};

#endif
