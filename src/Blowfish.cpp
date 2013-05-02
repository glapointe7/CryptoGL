#include "Blowfish.hpp"
#include "exceptions/BadKeyLength.hpp"

// Clé entre 48 bits et 448 bits.
void Blowfish::setKey(const BytesContainer &key)
{
   if(key.size() < 6 || key.size() > 56)
   {
      throw BadKeyLength("Your key length have to be between 6 bytes and 56 bytes.", key.size());
   }
   
   this->key = key;
}

Blowfish::UInt64Container Blowfish::getKeySchedule()
{
   UInt64Container subkeys(18, 0);
   //const uint32_t key_len = key.size();
   
   // on subdivise la clé en blocs de 32 bits.
   
   
   return subkeys;
}

uint64_t Blowfish::F(const uint64_t &data, const uint64_t &subkey) const
{
   return 0;
}

const Blowfish::BytesContainer Blowfish::encode(const BytesContainer &clear_text)
{
   BytesContainer crypted;

   return crypted;
}

const Blowfish::BytesContainer Blowfish::decode(const BytesContainer &cipher_text)
{
   BytesContainer decrypted;

   return decrypted;
}