
#ifndef STRINGCIPHER_HPP
#define STRINGCIPHER_HPP

#include "Cipher.hpp"

#include <string>

class StringCipher : public Cipher<std::string>
{
public:
   typedef std::string ClassicalType; 
      
   static void save(const std::string &filename, const ClassicalType &data);
   static const ClassicalType load(const std::string &filename);
   
   void encodeFile(const std::string &clear_text_file, const std::string &cipher_text_file);
   void decodeFile(const std::string &clear_text_file, const std::string &cipher_text_file);
   
   void eraseBadCharacters(ClassicalType &text) const;
   
   virtual void setAlpha(const ClassicalType &alpha);
   const ClassicalType getAlpha() const;

protected: 
   StringCipher();
   virtual ~StringCipher() {}

   virtual const ClassicalType encode(const ClassicalType &) = 0;
   virtual const ClassicalType decode(const ClassicalType &) = 0;
   
   const ClassicalType appendChars(const ClassicalType &data, const uint32_t to_add, const char c);
   const char badAlphaFound(const ClassicalType &text) const;
   void checkAlpha(const ClassicalType &alpha) const;
   
   ClassicalType alpha;
};

#endif
