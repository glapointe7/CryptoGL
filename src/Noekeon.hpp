/*
 * Source : http://gro.noekeon.org/Noekeon-spec.pdf
 */
#ifndef NOEKEON_HPP
#define NOEKEON_HPP

#include "BlockCipher.hpp"
#include "BigEndian.hpp"

class Noekeon : public BlockCipher<uint32_t, UInt32Vector, 16, BigEndian32>
{
public:
   /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
   Noekeon(const BytesVector &key, const OperationModes mode, const BytesVector &IV) 
      : BlockCipher(mode, 16, IV) { setKey(key); }
      
   explicit Noekeon(const BytesVector &key) 
      : Noekeon(key, OperationModes::ECB, {}) {}
   
   Noekeon(const BytesVector &key, const OperationModes mode) 
      : Noekeon(key, mode, {}) {}
      
   /* Constructor with a vector of IV only for the mode CTR. */
   Noekeon(const BytesVector &key, const IV_Vector &IV) 
      : BlockCipher(16, IV) { setKey(key); }
   
   virtual void setKey(const BytesVector &key) final;
   
private:
   virtual void generateSubkeys() final;
   virtual void generateInverseSubkeys() final;
   virtual const UInt32Vector encodeBlock(const UInt32Vector &input) final;
   virtual const UInt32Vector decodeBlock(const UInt32Vector &input) final;
   
   static void applyGamma(UInt32Vector &state);
   void applyTheta(UInt32Vector &state) const;
   
   /* Apply theta on the subkeys with null vector for decoding. */
   void applyTheta();
   
   static void applyPi1(UInt32Vector &state);
   static void applyPi2(UInt32Vector &state);
   void applyRound(UInt32Vector &state, const uint8_t constant1, const uint8_t constant2);
   
   static constexpr uint32_t round_constants[17] = {
      0x00000080, 0x0000001b, 0x00000036, 0x0000006c,
      0x000000d8, 0x000000ab, 0x0000004d, 0x0000009a,
      0x0000002f, 0x0000005e, 0x000000bc, 0x00000063,
      0x000000c6, 0x00000097, 0x00000035, 0x0000006a,
      0x000000d4
   };
};

#endif