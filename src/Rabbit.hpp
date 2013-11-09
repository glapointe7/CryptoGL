/*
 * Source : http://tools.ietf.org/html/rfc4503#appendix-A.1
 */
#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "SynchronousStreamCipher.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "LittleEndian.hpp"

#include <vector>
#include <array>

class Rabbit : public SynchronousStreamCipher<UInt32Vector, LittleEndian32, 16>
{   
public:
   /* Constructor with an IV. */
   Rabbit(const BytesVector &key, const BytesVector &IV) { setIV(IV); setKey(key); }
   
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
   
   std::array<uint32_t, 8> states;
   std::array<uint32_t, 8> counters;
   
   BytesVector IV;

   bool counter_carry_bit = 0;
   
   static constexpr std::array<uint32_t, 8> A = {
      {0x4D34D34D, 0xD34D34D3, 0x34D34D34, 0x4D34D34D,
      0xD34D34D3, 0x34D34D34, 0x4D34D34D, 0xD34D34D3}
   };
};

#endif