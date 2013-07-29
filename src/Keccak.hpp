/*
 * Source : http://keccak.noekeon.org/Keccak-implementation-3.2.pdf
 */
#ifndef KECCAK_HPP
#define	KECCAK_HPP

#include "SpongeFunction.hpp"

class Keccak : public SpongeFunction<uint64_t>
{
protected: 
   typedef typename SpongeFunction<uint64_t>::BytesContainer BytesContainer;
   
   /* Default constructor : output size of 512 bits, with capacity of 576 bits 
      bitrate of 1024 bits and 24 rounds. */
   Keccak() : SpongeFunction() {}
   
   Keccak(const uint16_t hash_size, const uint16_t c, const uint16_t r)
      : SpongeFunction(hash_size, c, r, 24) {}
   virtual ~Keccak() {}
      
private:
   /* Keccak-f permutation function. */
   virtual void F() final;
   
   /* Apply the absorbing phase from the sponge construction using the padded input message.*/
   virtual void applyAbsorbingPhase(const BytesContainer &padded_message) final;
   
   /* Apply the squeezing phase from the sponge construction and return the final output hash. */
   virtual const BytesContainer applySqueezingPhase() final;
   
   static constexpr uint8_t getIndex(const uint8_t x, const uint8_t y) { return (x % 5) + (5 * (y % 5)); }
   
   /* Apply thete, rho, pi, chi and iota steps from the specs. */
   void applyRound(const uint8_t round_index);
   
   static const uint64_t round_constants[24];
   static const uint8_t rotation_offsets[5][5];
};

class Keccak224 : public Keccak
{
public:
   Keccak224() : Keccak(224, 1152, 448) {}
};

class Keccak256 : public Keccak
{
public:
   Keccak256() : Keccak(256, 1088, 512) {}
};

class Keccak384 : public Keccak
{
public:
   Keccak384() : Keccak(384, 832, 768) {}
};

class Keccak512 : public Keccak
{
public:
   Keccak512() : Keccak() {}
};

#endif