/*
 * Source : http://www.cs.ucdavis.edu/~rogaway/papers/seal.pdf
 */
#ifndef SEAL_HPP
#define SEAL_HPP

#include "PseudoRandomFunction.hpp"
#include "StreamCipher.hpp"

class SEAL : public PseudoRandomFunction<uint32_t, UInt32Vector>, public StreamCipher
{
public:
   /* Constructor (Pseudo-random): receives the 160-bit key, a seed and the output size desired in bytes. */
   SEAL(const BytesVector &key, const uint32_t seed, const uint16_t output_size)
           : PseudoRandomFunction(seed), output_size(output_size) { setKey(key); }
   
   /* Constructor (Stream cipher): receives the 160-bit key for encryption. */
   explicit SEAL(const BytesVector &key) { setKey(key); }
           
   virtual const BytesVector encode(const BytesVector &message) final;
   
   virtual const UInt32Vector generate() final;
   
   virtual void setKey(const BytesVector &key) final;
   
private:
   /* Initialise the vectors A and registers from a given value and an index. */
   void initialize(const uint8_t index, UInt32Vector &A, UInt32Vector &registers) const;
   virtual void generateSubkeys() final;
   
   /* */
   uint32_t gamma(const uint32_t current_index, uint32_t &previous_index);
   
   /* Subkeys tables : T is 2048 bytes, S is 1024 bytes and R is 16 * (L / 1024) bytes. */
   UInt32Vector R, S, T;
   
   /* IV defined by the 160-bit key for the SHA-1 encryption. */
   UInt32Vector IV;
   
   /* The hash from SHA-1. */
   UInt32Vector H;
   
   uint16_t output_size;
};

#endif