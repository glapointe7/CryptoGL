
#ifndef ASYMMETRICCIPHER_HPP
#define	ASYMMETRICCIPHER_HPP

#include "Cipher.hpp"

#include <string>
#include <vector>

class AsymmetricCipher : public Cipher<std::vector<uint64_t>, std::vector<uint8_t> >
{
protected:
   typedef std::vector<uint64_t> UInt64Container;
   typedef std::vector<uint8_t> BytesContainer;
   
   virtual ~AsymmetricCipher() {}
   virtual const UInt64Container encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const UInt64Container &) = 0;
   virtual void setPrivateKey(const UInt64Container &) = 0;
   virtual void buildPublicKey() = 0;

public:      
   const UInt64Container getPublicKey() const { return public_key; }
   const UInt64Container getPrivateKey() const { return private_key; }

   static const std::string dwordToString(const UInt64Container &data);

protected:
   UInt64Container public_key;
   UInt64Container private_key;
};

#endif