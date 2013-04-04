
#ifndef BUFFERCIPHER_H
#define	BUFFERCIPHER_H

#include "Cipher.hpp"

#include <vector>
#include <string>

typedef std::vector<unsigned char> Bytes_Container;

class BufferCipher : public Cipher<Bytes_Container>
{
public:
   BufferCipher();
   virtual ~BufferCipher() {}

   void save(const std::string &, const Bytes_Container &);
   Bytes_Container load(const std::string &);

   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   std::string hexDigest(const Bytes_Container &data) const;

protected:
   
private:
   virtual Bytes_Container encode(const Bytes_Container &) = 0;
   virtual Bytes_Container decode(const Bytes_Container &) = 0;
};

#endif

