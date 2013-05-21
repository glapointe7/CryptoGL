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
   typedef std::vector<std::vector<uint8_t> > SBox;

public:
   DES() : Feistel(OperationModes::ECB) {}
   explicit DES(const OperationModes mode) : Feistel(mode) {}
   
   virtual const BytesContainer encode(const BytesContainer &) final;
   virtual const BytesContainer decode(const BytesContainer &) final;

   virtual void setKey(const BytesContainer &key) final;
   //uint8_t getParityBits();


private:
   virtual const UInt64Container getKeySchedule() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &data, 
           const UInt64Container &subkeys, const int8_t lower_round) final;
   uint64_t getSubstitution(const uint64_t &key_mixed) const;

   // Feistel function F.
   uint64_t F(const uint64_t &data, const uint64_t &subkey) const final;

   static const SBox IP;
   static const SBox IP_inverse;
   static const SBox E;
   static const SBox P;
   static const std::vector<SBox> S;
   static const SBox PC1;
   static const SBox PC2;
   static const BytesContainer rot_table;
};

#endif