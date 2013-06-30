/*
 * http://en.wikipedia.org/wiki/Data_Encryption_Standard
 * http://www.orlingrabbe.com/des.htm
 */

#ifndef DES_HPP
#define	DES_HPP

#include "Feistel.hpp"

#include <vector>

class DES : public Feistel
{
public:
   explicit DES(const BytesContainer &key) : Feistel(OperationModes::ECB) { setKey(key); }
   explicit DES(const BytesContainer &key, const OperationModes mode) : Feistel(mode) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   //uint8_t getParityBits();

private:
   typedef std::vector<std::vector<uint8_t> > SBox;
   
   virtual void setKey(const BytesContainer &key) final;
   virtual const UInt64Container getKeySchedule() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &data, 
           const UInt64Container &subkeys, const uint8_t lower_round) final;
   uint64_t getSubstitution(const uint64_t &key_mixed) const;

   /* Feistel function F. */
   virtual uint64_t F(const uint64_t &data, const uint64_t &subkey) const final;

   static const uint8_t IP[64];
   static const uint8_t IP_inverse[64];
   static const uint8_t E[48];
   static const uint8_t P[32];
   static const uint8_t S[8][4][16];
   static const uint8_t PC1[56];
   static const uint8_t PC2[48];
   static const uint8_t rot_table[16];
};

#endif