
#ifndef SALSA20_HPP
#define SALSA20_HPP

#include "StreamCipher.hpp"
#include "PseudoRandomFunction.hpp"

class Salsa20 : public StreamCipher, public PseudoRandomFunction<BytesVector, UInt32Vector>
{
public:
   Salsa20(const BytesVector &key, const BytesVector &IV) { setIV(IV); setKey(key); }
   
   virtual const BytesVector encode(const BytesVector &message) final;
   virtual void setKey(const BytesVector &key) final;
   
   /* Generate 64 bytes of keystream. */
   virtual const UInt32Vector generate() final;
   
private:
   void setIV(const BytesVector &IV);
   
   virtual void generateSubkeys() final;
      
   static UInt32Vector quarterRound(const UInt32Vector &Y);
   static UInt32Vector rowRound(const UInt32Vector &Y);
   static UInt32Vector columnRound(const UInt32Vector &Y);
   static UInt32Vector doubleRound(const UInt32Vector &Y);
   
   BytesVector IV;
   BytesVector subkeys;
   uint64_t counter = 0;
   
   static constexpr uint8_t sigma[4][4] = {
      {101, 120, 112, 97}, {110, 100, 32, 51}, {50, 45, 98, 121}, {116, 101, 32, 107}
   };
   
   static constexpr uint8_t tau[4][4] = {
      {101, 120, 112, 97}, {110, 100, 32, 49}, {54, 45, 98, 121}, {116, 101, 32, 107}
   };
};

#endif