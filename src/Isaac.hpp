/*
 * Source : http://burtleburtle.net/bob/rand/isaac.html
 */
#ifndef ISAAC_HPP
#define ISAAC_HPP

#include "PseudoRandomFunction.hpp"

#include <array>

class Isaac : public PseudoRandomFunction<UInt32Vector, UInt32Vector>
{
public:
   /* Constructor with seed given. */
   explicit Isaac(const UInt32Vector &seed) 
      : PseudoRandomFunction(seed), rand_mem(seed) { initialize(); }
   
   /* Constructor with no seed given. */
   Isaac() : Isaac(UInt32Vector(0)) {}
   
   virtual UInt32Vector generate() final;
   
private:
   void initialize();
   static void shuffle(std::array<uint32_t, 8> &tmp);
   void randFill(std::array<uint32_t, 8> &tmp, const UInt32Vector &rnd);
   uint32_t applyRound(const uint32_t accum_shifted, const uint16_t index);
   
   uint32_t counter = 0;
   uint32_t accumulator = 0;
   uint32_t previous = 0;
   
   /* The internal state. If no seed is given, keystream will be filled from rand_mem. */
   UInt32Vector rand_mem;
   UInt32Vector keystream;
   
   /* Shift constants used in shuffle. */
   static constexpr std::array<uint8_t, 8> shift = {{11, 8, 10, 8, 2, 16, 4, 9}};
};

#endif