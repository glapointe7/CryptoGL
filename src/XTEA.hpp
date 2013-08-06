/*
 * Source : http://www.cix.co.uk/~klockstone/xtea.pdf
 */
#ifndef XTEA_HPP
#define	XTEA_HPP

#include "Feistel.hpp"

class XTEA : public Feistel<uint32_t, std::vector<uint32_t> >
{
public:
   explicit XTEA(const BytesContainer &key) : Feistel<uint32_t, std::vector<uint32_t> >(OperationModes::ECB, 32, 8) { setKey(key); }
   XTEA(const BytesContainer &key, const OperationModes mode) : Feistel<uint32_t, std::vector<uint32_t> >(mode, 32, 8) { setKey(key); }
   
   virtual void setKey(const BytesContainer &key) final;
   
private:
   static constexpr uint32_t delta = 0x9E3779B9;
   
   virtual void generateSubkeys() final;
   virtual const UInt32Container getIntegersFromInputBlock(const BytesContainer &block) const final;
   virtual const UInt32Container encodeBlock(const UInt32Container &input) final;
   virtual const UInt32Container decodeBlock(const UInt32Container &input) final;
   virtual const BytesContainer getOutputBlock(const UInt32Container &int_block) final;
   
   virtual uint32_t F(const uint32_t half_block, const uint32_t subkey) const final;
   virtual void encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const final;
};

#endif