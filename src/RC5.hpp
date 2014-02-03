/*
 * Source : http://people.csail.mit.edu/rivest/Rivest-rc5.pdf
 * RC5 is a Feistel network without a F function.
 */
#ifndef RC5_HPP
#define RC5_HPP

#include "Feistel.hpp"
#include "Endian.hpp"

class RC5 : public Feistel<uint32_t, UInt32Vector, 8, LittleEndian32>
{
public:
   /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
   RC5(const BytesVector &key, const OperationModes mode, const BytesVector &IV) 
      : Feistel(mode, 12, IV) { setKey(key); }
      
   explicit RC5(const BytesVector &key) 
      : RC5(key, OperationModes::ECB, {}) {}
   
   RC5(const BytesVector &key, const OperationModes mode) 
      : RC5(key, mode, {}) {}
   
   void setKey(const BytesVector &key) override;
   
private:
   //const uint8_t block_size = 64;
   //static const uint16_t P16 = 0xb7e1;
   //static const uint16_t Q16 = 0x9e37;
   static constexpr uint32_t P32 = 0xb7e15163;
   static constexpr uint32_t Q32 = 0x9e3779b9;
   //static const uint64_t P64 = 0xb7e151628aed2a6b;
   //static const uint64_t Q64 = 0x9e3779b97f4a7c15;
   
   void generateSubkeys() override;
   UInt32Vector encodeBlock(const UInt32Vector &input) override;
   UInt32Vector decodeBlock(const UInt32Vector &input) override;
   
   uint32_t F(const uint32_t half_block, const uint8_t) const override { return 0; }
   void encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const override;
   void decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const override;
};

#endif