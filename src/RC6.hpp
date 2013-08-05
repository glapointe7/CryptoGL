/*
 * Source : http://people.csail.mit.edu/rivest/pubs/RRSY98.pdf
 */
#ifndef RC6_HPP
#define	RC6_HPP

#include "Feistel.hpp"

class RC6 : public Feistel //<uint32_t, uint32_t>
{
public:
   explicit RC6(const BytesContainer &key) : Feistel(OperationModes::ECB, 20, 16) { setKey(key); }
   RC6(const BytesContainer &key, const OperationModes mode) : Feistel(mode, 20, 16) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
private:
   static constexpr uint32_t P32 = 0xb7e15163;
   static constexpr uint32_t Q32 = 0x9e3779b9;
   
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &block, const bool to_encode) final;
   
   virtual uint64_t F(const uint64_t &X, const uint64_t&) const final;
   virtual void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
};

#endif