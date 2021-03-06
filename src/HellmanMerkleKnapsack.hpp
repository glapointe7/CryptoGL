/*
 * Source : http://paper.ijcsns.org/07_book/201105/20110502.pdf
 */
#ifndef HELLMANMERKLEKNAPSACK_HPP
#define	HELLMANMERKLEKNAPSACK_HPP

#include "AsymmetricCipher.hpp"

#include "exceptions/BadKey.hpp"
#include "exceptions/BadKeyLength.hpp"

namespace CryptoGL
{
    class HellmanMerkleKnapsack : public AsymmetricCipher
    {
    public:
        /* The private key (W, div, mod) need to be given where W is the super incresing sequence and div (mod mod). */
        HellmanMerkleKnapsack(const BigIntVector &sequence, const BigInteger &div, const BigInteger &mod);

        BigIntVector encode(const BytesVector &message) override;
        BytesVector decode(const BigIntVector &cipher) override;

    private:
        void makePublicKey() override;
        static BigInteger isSuperIncresing(const BigIntVector &sequence);
        uint8_t makePlainByte(BigInteger value) const;

        using SequenceNotSuperIncreasing = BadKey;
        using IntegersNotCoprimes = BadKey;
        using DividendGreaterThanModulo = BadKey;
        using ModuloNotGreaterThanSequenceSum = BadKey;
        using BadSequenceLength = BadKeyLength;

        /* Super increasing sequence. */
        BigIntVector sequence;

        /* div modulo mod */
        BigInteger div;
        BigInteger mod;

        BigIntVector public_key;
    };
}

#endif