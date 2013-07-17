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
   explicit DES(const BytesContainer &key) : Feistel(OperationModes::ECB, 16, 8) { setKey(key); }
   DES(const BytesContainer &key, const OperationModes mode) : Feistel(mode, 16, 8) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   //uint8_t getParityBits();

private:
   typedef std::vector<std::vector<uint8_t> > SBox;
   
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &data, const bool to_encode) final;
   
   virtual uint64_t F(const uint64_t &data, const uint64_t &subkey) const final;
   virtual void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
   
   uint64_t getSubstitution(const uint64_t &key_mixed) const;
   
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