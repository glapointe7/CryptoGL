
#ifndef STRINGCIPHER_H
#define STRINGCIPHER_H

#include <string>

#include "Cipher.hpp"

class StringCipher : public Cipher<std::string, std::string>
{
public:
   StringCipher();
   virtual ~StringCipher() {}

   virtual std::string encode(const std::string &) = 0;
   virtual std::string decode(const std::string &) = 0;
   
   static void save(const std::string &, const std::string &);
   static std::string load(const std::string &);
   
   void eraseBadCharacters(std::string &text) const;
   
   void setAlpha(const std::string &letters);
   std::string getAlpha() const;

protected:
   std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

#endif // STRINGCIPHER_H
