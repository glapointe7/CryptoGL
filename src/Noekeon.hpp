/*
 * Source : http://gro.noekeon.org/Noekeon-spec.pdf
 */
#ifndef NOEKEON_HPP
#define NOEKEON_HPP

#include "BlockCipher.hpp"
#include "BigEndian.hpp"

class Noekeon : public BlockCipher<uint32_t, std::vector<uint32_t>, 16, BigEndian32>
{
public:
   explicit Noekeon(const BytesContainer &key) : BlockCipher(OperationModes::ECB, 16) { setKey(key); }
   Noekeon(const BytesContainer &key, const OperationModes mode) : BlockCipher(mode, 16) { setKey(key); }
   
   virtual void setKey(const BytesContainer &key) final;
   
private:
   virtual void generateSubkeys() final;
   virtual void generateInverseSubkeys() final;
   virtual const UInt32Container getIntegersFromInputBlock(const BytesContainer &block) const final;
   virtual const UInt32Container encodeBlock(const UInt32Container &input) final;
   virtual const UInt32Container decodeBlock(const UInt32Container &input) final;
   
   static void applyGamma(UInt32Container &state);
   void applyTheta(UInt32Container &state) const;
   
   /* Apply theta on the subkeys with null vector for decoding. */
   void applyTheta();
   
   static void applyPi1(UInt32Container &state);
   static void applyPi2(UInt32Container &state);
   void applyRound(UInt32Container &state, const uint8_t constant1, const uint8_t constant2);
   
   static constexpr uint32_t round_constants[17] = {
      0x00000080, 0x0000001b, 0x00000036, 0x0000006c,
      0x000000d8, 0x000000ab, 0x0000004d, 0x0000009a,
      0x0000002f, 0x0000005e, 0x000000bc, 0x00000063,
      0x000000c6, 0x00000097, 0x00000035, 0x0000006a,
      0x000000d4
   };
};

#endif