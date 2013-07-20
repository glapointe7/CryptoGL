/*
 * Source : http://www.cix.co.uk/~klockstone/xtea.pdf
 */
#ifndef XTEA_HPP
#define	XTEA_HPP

#include "Feistel.hpp"

class XTEA : public Feistel
{
public:
   explicit XTEA(const BytesContainer &key) : Feistel(OperationModes::ECB, 32, 8) { setKey(key); }
   XTEA(const BytesContainer &key, const OperationModes mode) : Feistel(mode, 32, 8) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
private:
   static const uint32_t delta = 0x9E3779B9;
   
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &block, const bool to_encode) final;
   
   virtual uint64_t F(const uint64_t &X, const uint64_t &subkey) const final;
   virtual void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
};

#endif