
#ifndef ASYMMETRICCIPHER_HPP
#define	ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class AsymmetricCipher : public Cipher<std::vector<unsigned long>, std::vector<unsigned char> >
{
public:
   
   typedef std::vector<unsigned long> Numbers;
   typedef std::vector<unsigned char> BytesContainer;
   
   virtual const Numbers encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const Numbers &) = 0;
   
   Numbers getPublicKey() const;
   virtual void setPublicKey() = 0;
   virtual void setPrivateKey(const Numbers &) = 0;
   
   static void save(const std::string &, const std::string &);
   static std::string load(const std::string &);
   
   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   std::string hexDigest(const BytesContainer &) const;
   std::string toString(const Numbers &) const;
   BytesContainer toBytes(const std::string &);
   
protected:
   Numbers public_key;
   Numbers private_key;
};

#endif