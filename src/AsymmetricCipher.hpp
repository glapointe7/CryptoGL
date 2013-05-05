
#ifndef ASYMMETRICCIPHER_HPP
#define	ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class AsymmetricCipher : public Cipher<std::vector<uint64_t>, std::vector<unsigned char> >
{
   // Hide Warning : "hides overloaded virtual function [-Woverloaded-virtual]"
   //using Cipher::decode;
   
public:
   
   typedef std::vector<uint64_t> UInt64Container;
   typedef std::vector<unsigned char> BytesContainer;
   
   virtual const UInt64Container encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const UInt64Container &) = 0;
   
   const UInt64Container getPublicKey() const;
   virtual void setPublicKey() = 0;
   virtual void setPrivateKey(const UInt64Container &) = 0;
   
   static void save(const std::string &filename, const std::string &data);
   static std::string load(const std::string &filename);
   
   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   static const std::string hexDigest(const BytesContainer &data);
   static const std::string dwordToString(const UInt64Container &data);
   static const std::string byteToString(const BytesContainer &data);
   static const BytesContainer stringToBytes(const std::string &data);
   
protected:
   UInt64Container public_key;
   UInt64Container private_key;
};

#endif