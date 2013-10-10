/*
 * Source : http://www.cs.ucdavis.edu/~rogaway/papers/seal.pdf
 */
#ifndef SEAL_HPP
#define SEAL_HPP

#include "SynchronousStreamCipher.hpp"
#include "PseudoRandomFunction.hpp"

class SEAL : public SynchronousStreamCipher<UInt32Vector, BigEndian32>, 
        PseudoRandomFunction<uint32_t, UInt32Vector>
{
public:   
   /* Constructor (Pseudo-random): receives the 160-bit key, a seed and the output size desired in bytes. */
   SEAL(const BytesVector &key, const uint32_t seed, const uint16_t output_size)
           : SynchronousStreamCipher(1024), PseudoRandomFunction(seed), output_size(output_size) { setKey(key); }
   
   virtual const UInt32Vector generate() final;
   
   virtual void setKey(const BytesVector &key) final;
   
private:
   /* Only used by the encode function. */
   virtual UInt32Vector generateKeystream() final;
   
   /* Initialise the vectors A and registers from a given value and an index. */
   void initialize();
   
   virtual void keySetup() final;
   
   /* Define an IV with SHA-1 and return the hash truncated. */
   uint32_t gamma(const uint32_t current_index, uint32_t &previous_index);
   
   /* Subkeys tables : T is 2048 bytes, S is 1024 bytes and R is 16 * (L / 1024) bytes. */
   UInt32Vector R, S, T;
   
   /* IV defined by the 160-bit key for the SHA-1 encryption. */
   UInt32Vector IV;
   
   /* The hash from SHA-1. */
   UInt32Vector H;
   
   /* States to update. */
   UInt32Vector state{0,0,0,0,0,0,0,0};
   
   uint64_t output_size;
   
   uint8_t counter = 0;
};

#endif