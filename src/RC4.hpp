#ifndef RC4_HPP
#define RC4_HPP

#include "StreamCipher.hpp"

#include <array>

class RC4 : public StreamCipher<BytesVector>
{
public:
   /* Constructor for the pseudo-random generator (usage of generate). */
   RC4(const BytesVector &key, const uint64_t &output_size) 
      : output_size(output_size) { setKey(key); }
      
   /* Constructor for the usage of encode / decode. */
   explicit RC4(const BytesVector &key) 
      : RC4(key, 0) {}
   
   BytesVector encode(const BytesVector &clear_text) override;
   
   BytesVector generateKeystream() override;
   
   void setKey(const BytesVector &key) override;
   
private:
   void keySetup() override;
   
   std::array<uint8_t, 256> subkeys;
   uint64_t output_size;
};

#endif