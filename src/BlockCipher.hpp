#ifndef BLOCKCIPHER_HPP
#define	BLOCKCIPHER_HPP

#include "SymmetricCipher.hpp"

#include "exceptions/BadKeyLength.hpp"

class BlockCipher : public SymmetricCipher
{
   using BadIVLength = BadKeyLength;
   
public:
   typedef std::vector<uint32_t> UInt32Container;
   typedef std::vector<uint64_t> UInt64Container;
   
   // Modes of operation to encode / decode a block cipher.
   // Default to ECB mode.
   enum class OpModes : uint8_t
   {
      ECB,     // electronic codebook
      CBC,     // Cipher-block chaining
      PCBC,    // Propagating cipher-block chaining
      CFB,     // Cipher feedback
      OFB,     // Output feedback
      CTR      // Counter
   };
   
   virtual ~BlockCipher() {}
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void setKey(const BytesContainer &) = 0;
   void setMode(const OpModes &Mode);
   void setIV(const BytesContainer &IV);
   
protected:
   virtual const UInt64Container getKeySchedule() = 0;
   
   OpModes Mode; 
   
   // Initialization vector.
   BytesContainer IV;
};

#endif

