/*
 * Source : http://gro.noekeon.org/Noekeon-spec.pdf
 */
#ifndef NOEKEON_HPP
#define NOEKEON_HPP

#include "BlockCipher.hpp"

#include <array>

namespace CryptoGL
{
    class Noekeon : public BlockCipher<uint32_t, UInt32Vector, 16, BigEndian32>
    {
    public:
        /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
        Noekeon(const BytesVector &key, const OperationModes mode, const BytesVector &IV)
            : BlockCipher(mode, 16, IV)
        {
            setKey(key);
        }
        
        explicit Noekeon(const BytesVector &key)
            : Noekeon(key, OperationModes::ECB, {}) { }
        
        Noekeon(const BytesVector &key, const OperationModes mode)
            : Noekeon(key, mode, {}) { }

    private:
        void generateSubkeys() override;
        void generateInverseSubkeys() override;
        void processEncodingCurrentBlock() override;
        void processDecodingCurrentBlock() override;

        static void setClassVector(const std::array<uint8_t, 4> &indices, UInt32Vector &V);

        void applyGamma();
        void applyThetaToState();

        /* Apply theta on the subkeys with null vector for decoding. */
        void applyThetaToSubkeys();

        void applyPiLeft();
        void applyPiRight();
        void applyRound(const uint8_t constant1, const uint8_t constant2);
        
        static constexpr std::array<uint32_t, 17> round_constants = {{
            0x00000080, 0x0000001b, 0x00000036, 0x0000006c,
            0x000000d8, 0x000000ab, 0x0000004d, 0x0000009a,
            0x0000002f, 0x0000005e, 0x000000bc, 0x00000063,
            0x000000c6, 0x00000097, 0x00000035, 0x0000006a,
            0x000000d4
         }};
    };
}

#endif