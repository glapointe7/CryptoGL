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

   virtual ~StringCipher() {}

   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;
   
   static void save(const std::string &, const ClassicalType &);
   static const ClassicalType load(const std::string &);
   
   void eraseBadCharacters(ClassicalType &) const;
   
   void setAlpha(const ClassicalType &letters);
   const ClassicalType getAlpha() const;

protected: 
   ClassicalType alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

#endif