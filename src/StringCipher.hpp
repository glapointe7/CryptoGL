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
   
   static const ClassicalType ALPHA;

   StringCipher() : alpha(ALPHA) {}
   virtual ~StringCipher() {}

   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;
   
   void save(const std::string &, const ClassicalType &);
   const ClassicalType load(const std::string &);
   
   void eraseBadCharacters(ClassicalType &) const;
   
   void setAlpha(const ClassicalType &letters);
   const ClassicalType getAlpha() const;

protected: 
   ClassicalType alpha;
};

#endif
