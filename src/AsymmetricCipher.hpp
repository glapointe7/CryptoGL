
#ifndef ASYMMETRICCIPHER_HPP
#define	ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class AsymmetricCipher : public Cipher<std::vector<uint64_t>, std::vector<unsigned char> >
{
   // Hide Warning : "hides overloaded virtual function [-Woverloaded-virtual]"
   using Cipher::decode;
   
public:
   
   typedef std::vector<uint64_t> UInt64Container;
   typedef std::vector<unsigned char> BytesContainer;
   
   virtual const UInt64Container encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const UInt64Container &) = 0;
   
   UInt64Container getPublicKey() const;
   virtual void setPublicKey() = 0;
   virtual void setPrivateKey(const UInt64Container &) = 0;
   
   static void save(const std::string &, const std::string &);
   static std::string load(const std::string &);
   
   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   static std::string hexDigest(const BytesContainer &);
   static std::string toString(const UInt64Container &);
   static BytesContainer toBytes(const std::string &);
   
protected:
   UInt64Container public_key;
   UInt64Container private_key;
};

#endif