/*
 * Source : http://cr.yp.to/snuffle/spec.pdf
 */
#ifndef SALSA20_HPP
#define SALSA20_HPP

#include "SynchronousStreamCipher.hpp"

#include "BigEndian.hpp"

class Salsa20 : public SynchronousStreamCipher<UInt32Vector, BigEndian32>
{
public:
   Salsa20(const BytesVector &key, const BytesVector &IV)
      : SynchronousStreamCipher(64) { setIV(IV); setKey(key); }
   
   /* Generate 64 bytes of keystream. */
   virtual UInt32Vector generateKeystream() final;
   
   void setIV(const BytesVector &IV);
   virtual void setKey(const BytesVector &key) final;
      
private:
   
   
   virtual void keySetup() final;
   void IVSetup();
      
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