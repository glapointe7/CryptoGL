/*
 * Source : http://www.cix.co.uk/~klockstone/xtea.pdf
 */
#ifndef XTEA_HPP
#define XTEA_HPP

#include "Feistel.hpp"

namespace CryptoGL
{
    class XTEA : public Feistel<uint32_t, uint64_t, 8, BigEndian64>
    {
    public:
        /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
        XTEA(const BytesVector &key, const OperationModes mode, const BytesVector &IV)
            : Feistel(mode, 32, IV)
        {
            setKey(key);
        }
        
        explicit XTEA(const BytesVector &key)
            : XTEA(key, OperationModes::ECB, {}) { }
        
        XTEA(const BytesVector &key, const OperationModes mode)
            : XTEA(key, mode, {}) { }

    private:
        void generateSubkeys() override;
        void processEncodingCurrentBlock() override;
        void processDecodingCurrentBlock() override;

        uint32_t F(const uint32_t half_block, const uint8_t round) const override;
        void encodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const override;
        void decodeFeistelRounds(uint32_t &L, uint32_t &R, const uint8_t) const override;
    };
}

#endif