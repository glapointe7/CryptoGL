/*
 * Source : http://www.cix.co.uk/~klockstone/xtea.pdf
 */
#ifndef XTEA_HPP
#define XTEA_HPP

#include "Feistel.hpp"
#include "BigEndian.hpp"

class XTEA : public Feistel<uint32_t, uint64_t, 8, BigEndian64>
{
public:
   /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
   XTEA(const BytesVector &key, const OperationModes mode, const BytesVector &IV) 
      : Feistel(mode, 32, IV) { setKey(key); }
      
   explicit XTEA(const BytesVector &key) 
      : XTEA(key, OperationModes::ECB, {}) {}
   
   XTEA(const BytesVector &key, const OperationModes mode) 
      : XTEA(key, mode, {}) {}
      
   /* Constructor with a vector of IV only for the mode CTR. */
   XTEA(const BytesVector &key, const IV_Vector &IV) 
      : Feistel(32, IV) { setKey(key); }
   
   virtual void setKey(const BytesVector &key) final;
   
private:
   static constexpr uint32_t delta = 0x9E3779B9;
   
   virtual void generateSubkeys() final;
   virtual const uint64_t encodeBlock(const uint64_t &input) final;
   virtual const uint64_t decodeBlock(const uint64_t &input) final;
   
   virtual const uint32_t F(const uint32_t half_block, const uint8_t round) const final;
   virtual void encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const final;
   virtual void decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const final;
};

#endif