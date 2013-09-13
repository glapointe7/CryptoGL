/*
 * Source : http://www.cs.ucdavis.edu/~rogaway/papers/seal.pdf
 */
#ifndef SEAL_HPP
#define SEAL_HPP

#include "PseudoRandomFunction.hpp"

class SEAL : public PseudoRandomFunction
{
public:
   /* Constructor : receives the 160-bit key and the output size desired in bytes. */
   SEAL(const BytesVector &key, const uint16_t L)
           : PseudoRandomFunction(L) { setKey(key); }
   
   virtual const UInt32Vector generate(const uint32_t value) final;
   
   virtual void setKey(const BytesVector &key) final;
   
private:
   /* Initialise the vectors A and registers from a given value and an index. */
   virtual void initialize(const uint32_t value, const uint8_t index, UInt32Vector &A, UInt32Vector &registers) final;
   virtual void generateSubkeys() final;
   
   /* */
   uint32_t gamma(const uint32_t current_index, uint32_t &previous_index);
   
   /* Subkeys tables : T is 2048 bytes, S is 1024 bytes and R is 16 * (L / 1024) bytes. */
   UInt32Vector R, S, T;
   
   /* IV defined by the 160-bit key for the SHA-1 encryption. */
   UInt32Vector IV;
   
   /* The hash from SHA-1. */
   UInt32Vector H;
};

#endif