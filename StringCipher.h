
#ifndef STRINGCIPHER_H
#define STRINGCIPHER_H

#include "Cipher.hpp"

#include <string>

class StringCipher : public Cipher<std::string>
{
public:
   typedef std::string String; // Change to CipherType or something

   virtual ~StringCipher() {}

   virtual const String encode(const String &) = 0;
   virtual const String decode(const String &) = 0;
   
   static void save(const std::string &, const std::string &);
   static const String load(const std::string &);
   
   void eraseBadCharacters(String &) const;
   
   void setAlpha(const String &letters);
   const String getAlpha() const;

protected: 
   String alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

#endif // STRINGCIPHER_H

