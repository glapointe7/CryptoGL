#ifndef ASYMMETRICCIPHER_HPP
#define	ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <vector>
#include <string>

class AsymmetricCipher : public Cipher<std::vector<unsigned long>, std::vector<unsigned char> >
{
public:
   
   typedef std::vector<unsigned long> Numbers;
   typedef std::vector<unsigned char> Bytes_Container;
   
   virtual ~AsymmetricCipher() {}
   
   virtual Numbers encode(const Bytes_Container &) = 0;
   virtual Bytes_Container decode(const Numbers &) = 0;
   
   Numbers getPrivateKey() const;
   virtual void setPrivateKey(const Numbers &) = 0;
   
   static void save(const std::string &, const std::string &);
   static std::string load(const std::string &);
   
   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   std::string hexDigest(const Bytes_Container &) const;
   std::string toString(const Numbers &) const;
   
protected:
   Numbers private_key;
};

#endif	/* ASYMMETRICCIPHER_HPP */