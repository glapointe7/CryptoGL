
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
   
   virtual Bytes_Container encode(const Bytes_Container &) = 0;
   virtual Bytes_Container decode(const Bytes_Container &) = 0;

   static void save(const std::string &, const Bytes_Container &);
   static Bytes_Container load(const std::string &);

   // Pour obtenir les octets sous forme d'une chaîne hexadécimale.
   std::string hexDigest(const Bytes_Container &data) const;
   Bytes_Container bytesContainer(const std::string &text) const;

protected:
   
private:
  
};

#endif

