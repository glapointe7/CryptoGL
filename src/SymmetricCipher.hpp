
#ifndef SYMMETRICCIPHER_HPP
#define	SYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class SymmetricCipher : public Cipher<std::vector<unsigned char>>
{
public:
   typedef std::vector<unsigned char> BytesContainer;

   virtual ~SymmetricCipher() {}

   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;

   virtual void setKey(const BytesContainer &) = 0;

   static void save(const std::string &, const BytesContainer &);
   static BytesContainer load(const std::string &);

   static const std::string hexDigest(const BytesContainer &);
   static const BytesContainer getBytesFromString(const std::string &);
   static const std::string getStringFromBytes(const BytesContainer &);

protected:
   BytesContainer key;
};

#endif	
