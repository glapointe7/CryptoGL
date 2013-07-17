/*
 * Source : http://csrc.nist.gov/groups/ST/toolkit/documents/skipjack/skipjack.pdf
 */
#ifndef SKIPJACK_HPP
#define SKIPJACK_HPP

#include "Feistel.hpp"

class Skipjack : public Feistel
{
public:
   explicit Skipjack(const BytesContainer &key) : Feistel(OperationModes::ECB, 32, 8) { setKey(key); }
   Skipjack(const BytesContainer &key, const OperationModes mode) : Feistel(mode, 32, 8) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
private:
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &block, const bool to_encode) final;
   
   virtual uint64_t F(const uint64_t &data, const uint64_t &subkey) const final;
   virtual void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t round) const final;
   virtual void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t round) const final;
   
   /* Rules A and B and their corresponding inverses. */
   void applyRuleA(uint16_t *input, const uint8_t round) const;
   void applyRuleB(uint16_t *input, const uint8_t round) const;
   void applyInverseRuleA(uint16_t *input, const uint8_t round) const;
   void applyInverseRuleB(uint16_t *input, const uint8_t round) const;
   
   static const uint8_t f_table[256];
};

#endif