/*
 * 
 */
#ifndef STRINGCIPHER_HPP
#define STRINGCIPHER_HPP

#include "Cipher.hpp"

#include <string>

class StringCipher : public Cipher<std::string>
{
public:
   typedef std::string ClassicalType; 
   
   StringCipher();
   virtual ~StringCipher() {}

   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;
   
   void save(const std::string &filename, const ClassicalType &data);
   const ClassicalType load(const std::string &filename) const;
   
   void eraseBadCharacters(ClassicalType &) const;
   
   void setAlpha(const ClassicalType &alpha);
   const ClassicalType getAlpha() const;

protected: 
   const ClassicalType appendChars(const ClassicalType &data, const uint32_t to_add, const char c);
   const char badAlphaFound(const ClassicalType &text) const;
   
   ClassicalType alpha;
};

#endif
