
#ifndef STRINGCIPHER_H
#define STRINGCIPHER_H

#include <string>

#include "Cipher.hpp"

class StringCipher : public Cipher<std::string>
{
public:
   StringCipher();
   ~StringCipher() {}

   virtual std::string encode(const std::string &) = 0;
   virtual std::string decode(const std::string &) = 0;
   
   void save(const std::string &);
   std::string load(const std::string &);
   
   void setAlpha(const std::string &letters);
   std::string getAlpha() const;

protected:
   std::string cipher_text;
   std::string clear_text;
   std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

#endif // STRINGCIPHER_H
