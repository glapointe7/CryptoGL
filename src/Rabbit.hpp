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
   explicit Rabbit(const BytesContainer &key);
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;

   void setIV(const BytesContainer &IV);

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
   UInt32Container states = {0, 0, 0, 0, 0, 0, 0, 0};
   UInt32Container counters = {0, 0, 0, 0, 0, 0, 0, 0};
   
   // Vecteur initial.
   BytesContainer IV;

   bool counter_carry_bit = 0;
};

#endif