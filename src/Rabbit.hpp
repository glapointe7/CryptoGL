/*
 * Algorithme : http://tools.ietf.org/html/rfc4503#appendix-A.1
 */
#ifndef RABBIT_HPP
#define	RABBIT_HPP

#include "StreamCipher.hpp"

#include "exceptions/BadKeyLength.hpp"

#include <vector>

class Rabbit : public StreamCipher
{   
public:
   explicit Rabbit(const BytesVector &key) { setKey(key); }
   
   virtual const BytesVector encode(const BytesVector &clear_text) final;
   virtual const BytesVector decode(const BytesVector &cipher_text) final;
   
   virtual void setKey(const BytesVector &key) final;

   void setIV(const BytesVector &IV);

private:
   using BadIVLength = BadKeyLength;
   using BadDataLength = BadKeyLength;
   
   virtual void keySetup() final;
   void IVSetup();
   void nextState();
   static uint32_t g(const uint32_t x);
   
   // Initialise le counters et le states en mettant le carry bit à 0.
   // States : divisé en 8 states de 32 bits chacun (uint32).
   // Counters : divisé en 8 counters de 32 bits chacun (uint32).
   // counter_carry_bit : 
   uint32_t states[8];
   uint32_t counters[8];
   
   // Initial vector.
   BytesVector IV;

   bool counter_carry_bit = 0;
};

#endif