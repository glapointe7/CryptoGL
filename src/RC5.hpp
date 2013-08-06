/*
 * Source : http://people.csail.mit.edu/rivest/Rivest-rc5.pdf
 * RC5 is a Feistel network without a F function.
 */
#ifndef RC5_HPP
#define RC5_HPP

#include "Feistel.hpp"

class RC5 : public Feistel<uint32_t, std::vector<uint32_t> >
{
public:
   explicit RC5(const BytesContainer &key) : Feistel<uint32_t, std::vector<uint32_t> >(OperationModes::ECB, 12, 8) { setKey(key); }
   RC5(const BytesContainer &key, const OperationModes mode) : Feistel<uint32_t, std::vector<uint32_t> >(mode, 12, 8) { setKey(key); }
   
   virtual void setKey(const BytesContainer &key) final;
   
private:
   //const uint8_t block_size = 64;
   //static const uint16_t P16 = 0xb7e1;
   //static const uint16_t Q16 = 0x9e37;
   static constexpr uint32_t P32 = 0xb7e15163;
   static constexpr uint32_t Q32 = 0x9e3779b9;
   //static const uint64_t P64 = 0xb7e151628aed2a6b;
   //static const uint64_t Q64 = 0x9e3779b97f4a7c15;
   
   virtual void generateSubkeys() final;
   virtual const UInt32Container getIntegersFromInputBlock(const BytesContainer &block) const final;
   virtual const UInt32Container encodeBlock(const UInt32Container &input) final;
   virtual const UInt32Container decodeBlock(const UInt32Container &input) final;
   virtual const BytesContainer getOutputBlock(const UInt32Container &int_block) final;
   
   virtual uint32_t F(const uint32_t half_block, const uint32_t) const final { return 0; }
   virtual void encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const final;
};

#endif