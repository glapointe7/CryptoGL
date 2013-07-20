/*
 * Source : http://people.csail.mit.edu/rivest/Rivest-rc5.pdf
 * RC5 is a Feistel network without a F function.
 */
#ifndef RC5_HPP
#define RC5_HPP

#include "Feistel.hpp"

class RC5 : public Feistel
{
public:
   explicit RC5(const BytesContainer &key) : Feistel(OperationModes::ECB, 12, 8) { setKey(key); }
   RC5(const BytesContainer &key, const OperationModes mode) : Feistel(mode, 12, 8) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
private:
   //const uint8_t block_size = 64;
   //static const uint16_t P16 = 0xb7e1;
   //static const uint16_t Q16 = 0x9e37;
   static const uint32_t P32 = 0xb7e15163;
   static const uint32_t Q32 = 0x9e3779b9;
   //static const uint64_t P64 = 0xb7e151628aed2a6b;
   //static const uint64_t Q64 = 0x9e3779b97f4a7c15;
   
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &block, const bool to_encode) final;
   
   virtual uint64_t F(const uint64_t &X, const uint64_t &Y) const final { return 0; }
   virtual void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
};

#endif