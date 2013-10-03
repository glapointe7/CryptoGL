/*
 * Algorithme : http://tools.ietf.org/html/rfc4503#appendix-A.1
 */
#ifndef RABBIT_HPP
#define RABBIT_HPP

#include "StreamCipher.hpp"

#include "exceptions/BadKeyLength.hpp"

#include <vector>

class Rabbit : public StreamCipher
{   
public:
   explicit Rabbit(const BytesVector &key) { setKey(key); }
   
   virtual const BytesVector encode(const BytesVector &clear_text) final;
   
   virtual void setKey(const BytesVector &key) final;

   void setIV(const BytesVector &IV);

private:
   using BadDataLength = BadKeyLength;
   
   virtual void generateSubkeys() final;
   void IVSetup();
   void nextState();
   static uint32_t g(const uint32_t x);
   
   uint32_t states[8];
   uint32_t counters[8];
   
   BytesVector IV;

   bool counter_carry_bit = 0;
};

#endif