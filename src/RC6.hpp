/*
 * Source : http://people.csail.mit.edu/rivest/pubs/RRSY98.pdf
 */
#ifndef RC6_HPP
#define	RC6_HPP

#include "Feistel.hpp"

class RC6 : public Feistel<uint64_t, std::vector<uint32_t> >
{
public:
   explicit RC6(const BytesContainer &key) : Feistel(OperationModes::ECB, 20, 16) { setKey(key); }
   RC6(const BytesContainer &key, const OperationModes mode) : Feistel(mode, 20, 16) { setKey(key); }
   
   virtual void setKey(const BytesContainer &key) final;
   
private:
   static constexpr uint32_t P32 = 0xb7e15163;
   static constexpr uint32_t Q32 = 0x9e3779b9;
   
   virtual void generateSubkeys() final;
   virtual const UInt32Container getIntegersFromInputBlock(const BytesContainer &block) const final;
   virtual const UInt32Container encodeBlock(const UInt32Container &input) final;
   virtual const UInt32Container decodeBlock(const UInt32Container &input) final;
   virtual const BytesContainer getOutputBlock(const UInt32Container &int_block) final;
   
   virtual const uint64_t F(const uint64_t half_block, const uint8_t) const final;
   virtual void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const final;
};

#endif