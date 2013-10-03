#ifndef RC4_HPP
#define RC4_HPP

#include "StreamCipher.hpp"
#include "PseudoRandomFunction.hpp"

class RC4 : public StreamCipher, PseudoRandomFunction<uint8_t, BytesVector>
{
public:
   /* Constructor for the pseudo-random generator (usage of generate). */
   RC4(const BytesVector &key, const uint64_t &output_size) 
      : output_size(output_size) { setKey(key); }
      
   /* Constructor for the usage of encode / decode. */
   explicit RC4(const BytesVector &key) 
      : RC4(key, 0) {}
   
   virtual const BytesVector encode(const BytesVector &clear_text) final;
   
   virtual const BytesVector generate();
   
   virtual void setKey(const BytesVector &key) final;
   
private:
   virtual void generateSubkeys() final;
   
   uint8_t subkeys[256];
   uint64_t output_size;
};

#endif