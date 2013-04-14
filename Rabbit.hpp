/*
 * Algorithme : http://tools.ietf.org/html/rfc4503#appendix-A.1
 */
#ifndef RABBIT_HPP
#define	RABBIT_HPP

#include "StreamCipher.hpp"

#include <vector>

class Rabbit : public StreamCipher
{
   typedef std::vector<uint32_t> UInt32Bytes;
   typedef std::vector<uint16_t> UInt16Bytes;

public:
   BytesContainer encode(const BytesContainer &);
   BytesContainer decode(const BytesContainer &);

   void setKey(const BytesContainer &);
   void setIV(const BytesContainer &);

private:
   void initialize();
   void initializeCounters();
   void update();
   BytesContainer getOutputBytes();
   uint32_t g(const uint32_t state) const;
   
   // Initialise le counters et le states en mettant le carry bit à 0.
   // States : divisé en 8 states de 32 bits chacun (uint32).
   // Counters : divisé en 8 counters de 32 bits chacun (uint32).
   // counter_carry_bit : 
   UInt32Bytes states = {0, 0, 0, 0, 0, 0, 0, 0};
   UInt32Bytes counters = {0, 0, 0, 0, 0, 0, 0, 0};
   
   BytesContainer IV;

   bool counter_carry_bit = 0;

   //const uint64_t word_size = 0x100000000;
};

#endif

