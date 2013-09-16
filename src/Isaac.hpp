/*
 * Source : http://burtleburtle.net/bob/rand/isaac.html
 */
#ifndef ISAAC_HPP
#define ISAAC_HPP

#include "PseudoRandomFunction.hpp"


class Isaac : public PseudoRandomFunction<UInt32Vector, UInt32Vector>
{
public:
   /* Constructor with seed given. */
   explicit Isaac(const UInt32Vector &seed) 
      : PseudoRandomFunction(seed), rand_mem(seed) { initialize(); }
   
   /* Constructor with no seed given. */
   Isaac() : Isaac(UInt32Vector(0)) {}
   
   virtual const UInt32Vector generate() final;
   
private:
   void initialize();
   void shuffle(uint32_t *tmp) const;
   void randFill(uint32_t *tmp, const UInt32Vector &rnd);
   uint32_t applyRound(const uint32_t accum_shifted, const uint16_t index);
   
   uint32_t counter = 0;
   uint32_t accumulator = 0;
   uint32_t previous = 0;
   
   /* The internal state. If no seed is given, keystream will be filled from rand_mem. */
   UInt32Vector rand_mem;
   UInt32Vector keystream;
   
   /* Shift constants used in shuffle. */
   static constexpr uint8_t shift[8] = {11, 8, 10, 8, 2, 16, 4, 9};
};

#endif