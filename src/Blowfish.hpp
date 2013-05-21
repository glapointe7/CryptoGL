/*
 * Constants taken here : http://www.schneier.com/code/constants.txt
 * Blowfish is a network Feistel with 16 rounds.
 */
#ifndef BLOWFISH_HPP
#define	BLOWFISH_HPP

#include "Feistel.hpp"

#include <vector>

class Blowfish : public Feistel
{
public:
   Blowfish() : Feistel(OperationModes::ECB) {}
   explicit Blowfish(const OperationModes mode) : Feistel(mode) {}
   
   virtual const BytesContainer encode(const BytesContainer &) final;
   virtual const BytesContainer decode(const BytesContainer &) final;
   
   virtual void setKey(const BytesContainer &key) final;
   
private:
   typedef std::vector<uint32_t> SBox;
   
   virtual const UInt64Container getKeySchedule() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &data, 
           const UInt64Container &subkeys, const int8_t lower_round) final;
   
   virtual uint64_t F(const uint64_t &data, const uint64_t &) const final;
   virtual void processFeistelRounds(uint64_t &L, uint64_t &R, const UInt64Container &subkeys, 
           const int8_t lower_round, const int8_t round_max) final;

   std::vector<SBox> sboxes;
   static const std::vector<SBox> sbox;
   static const UInt64Container P;
};

#endif