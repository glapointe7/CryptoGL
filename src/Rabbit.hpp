/*
 * Algorithme : http://tools.ietf.org/html/rfc4503#appendix-A.1
 */
#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "SynchronousStreamCipher.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "LittleEndian.hpp"

#include <vector>

class Rabbit : public SynchronousStreamCipher<UInt32Vector, LittleEndian32>
{   
public:
   /* Constructor with an IV. */
   Rabbit(const BytesVector &key, const BytesVector &IV) 
      : SynchronousStreamCipher(16) { setIV(IV); setKey(key); }
   
   /* Constructor without IV. */
   explicit Rabbit(const BytesVector &key) 
      : Rabbit(key, BytesVector(0)) {}
   
   virtual UInt32Vector generateKeystream() final;
   
   virtual void setKey(const BytesVector &key) final;

   void setIV(const BytesVector &IV);

private:
   virtual void keySetup() final;
   void IVSetup();
   
   void nextState();
   static uint32_t g(const uint32_t x);
   
   uint32_t states[8];
   uint32_t counters[8];
   
   BytesVector IV;

   bool counter_carry_bit = 0;
   
   static constexpr uint32_t A[8] = {
      0x4D34D34D, 0xD34D34D3, 0x34D34D34, 0x4D34D34D,
      0xD34D34D3, 0x34D34D34, 0x4D34D34D, 0xD34D34D3
   };
};

#endif