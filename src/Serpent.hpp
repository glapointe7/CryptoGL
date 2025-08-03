#pragma once

#include "BlockCipher.hpp"
#include <array>

namespace CryptoGL
{
    using SBox = UInt32Vector(*)(const UInt32Vector &Y);

    class Serpent : public BlockCipher<uint32_t, UInt32Vector, 16, LittleEndian32>
    {
    public:
        // Constructor with IV for CBC, CFB, OFB modes
        Serpent(const BytesVector &key, const OperationModes mode, const BytesVector &IV);
        // Default constructor: ECB mode
        explicit Serpent(const BytesVector &key);
        // Constructor for ECB/CTR with no IV
        Serpent(const BytesVector &key, const OperationModes mode);

        void setKey(const BytesVector &key) override;

    private:
        void generateSubkeys() override;
        void processEncodingCurrentBlock() override;
        void processDecodingCurrentBlock() override;

        UInt32Vector padKey();
        void applyKeyMixing(const uint8_t round);

        static void printToConsole(const uint32_t x);

        static UInt32Vector S0(const UInt32Vector &Y);
        static UInt32Vector S1(const UInt32Vector &Y);
        static UInt32Vector S2(const UInt32Vector &Y);
        static UInt32Vector S3(const UInt32Vector &Y);
        static UInt32Vector S4(const UInt32Vector &Y);
        static UInt32Vector S5(const UInt32Vector &Y);
        static UInt32Vector S6(const UInt32Vector &Y);
        static UInt32Vector S7(const UInt32Vector &Y);

        static UInt32Vector SI0(const UInt32Vector &Y);
        static UInt32Vector SI1(const UInt32Vector &Y);
        static UInt32Vector SI2(const UInt32Vector &Y);
        static UInt32Vector SI3(const UInt32Vector &Y);
        static UInt32Vector SI4(const UInt32Vector &Y);
        static UInt32Vector SI5(const UInt32Vector &Y);
        static UInt32Vector SI6(const UInt32Vector &Y);
        static UInt32Vector SI7(const UInt32Vector &Y);
        
        //static UInt32Vector linearTransform(const UInt32Vector &block);
        void linearTransform();
        void inverseLinearTransform();

        const std::array<SBox, 8> S = {S0, S1, S2, S3, S4, S5, S6, S7};
        const std::array<SBox, 8> SI = {SI0, SI1, SI2, SI3, SI4, SI5, SI6, SI7};

        // S-boxes
        //static const std::array<std::array<uint8_t, 16>, 8> SBoxes;
        //static const std::array<std::array<uint8_t, 16>, 8> SBoxesInv;
    };
}