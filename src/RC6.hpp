/*
 * Source : http://people.csail.mit.edu/rivest/pubs/RRSY98.pdf
 */
#ifndef RC6_HPP
#define RC6_HPP

#include "Feistel.hpp"

namespace CryptoGL
{
    class RC6 : public Feistel<uint64_t, UInt32Vector, 16, LittleEndian32>
    {
    public:
        /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
        RC6(const BytesVector &key, const OperationModes mode, const BytesVector &IV)
            : Feistel(mode, 20, IV)
        {
            setKey(key);
        }
        
        explicit RC6(const BytesVector &key)
            : RC6(key, OperationModes::ECB,{}) { }
        
        RC6(const BytesVector &key, const OperationModes mode)
            : RC6(key, mode,{}) { }

        void setKey(const BytesVector &key) override;

    private:
        static constexpr uint32_t P32 = 0xb7e15163;
        static constexpr uint32_t Q32 = 0x9e3779b9;

        void generateSubkeys() override;
        UInt32Vector encodeBlock(const UInt32Vector &input) override;
        UInt32Vector decodeBlock(const UInt32Vector &input) override;

        uint64_t F(const uint64_t half_block, const uint8_t) const override;
        void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const override;
        void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const override;
    };
}

#endif