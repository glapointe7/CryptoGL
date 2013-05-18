/* MODES OF OPERATION
 * 
 * CBC : C_i = encode(P_i ^ C_i-1) with IV = C_0.
 *       P_i = decode(C_i) ^ C_i-1 with IV = C_0.
 * 
 * PCBC: C_i = encode(P_i ^ P_i-1 ^ C_i) with IV = C_0 ^ P_0.
 *       P_i = decode(C_i) ^ P_i-1 ^ C_i with IV = C_0 ^ P_0.
 * 
 * CFB : C_i = encode(C_i-1) ^ P_i with IV = C_0.
 *       P_i = encode(C_i-1) ^ C_i ^ C_i with IV = C_0.
 * 
 * OFB : C_i = P_i ^ O_i with IV = I_0 and O_i = encode(I_i) and I_i = O_i-1.
 *       P_i = C_i ^ O_i with IV = I_0.and O_i = encode(I_i) and I_i = O_i-1.
 */
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