#include "HellmanMerkleKnapsack.hpp"

#include "MathematicalTools.hpp"

#include <algorithm>

using namespace CryptoGL;

HellmanMerkleKnapsack::HellmanMerkleKnapsack(const BigIntVector &sequence, const BigInteger &div, const BigInteger &mod)
{
    if (!Maths::areCoprimes(div, mod))
    {
        throw IntegersNotCoprimes("The modulo 'mod' and dividend 'div' have to be coprimes.");
    }

    if (div >= mod)
    {
        throw DividendGreaterThanModulo("Your modulo 'mod' has to be greater than your dividend 'div'.");
    }

    if (sequence.size() != 8)
    {
        throw BadSequenceLength("Your sequence's vector has to be of length 8.", sequence.size());
    }

    const BigInteger sum_sequence = isSuperIncresing(sequence);
    if (sum_sequence == 0)
    {
        throw SequenceNotSuperIncreasing("Your sequence's vector has to be a super increasing sequence.");
    }

    if (mod <= sum_sequence)
    {
        throw ModuloNotGreaterThanSequenceSum("Your modulo 'mod' has to be greater than the sum of the values in 'sequence'.");
    }

    this->div = div;
    this->mod = mod;
    this->sequence = sequence;

    makePublicKey();
}

BigInteger HellmanMerkleKnapsack::isSuperIncresing(const BigIntVector &sequence)
{
    BigInteger sum;
    for (const auto &number : sequence)
    {
        if (number <= sum)
        {
            return 0;
        }
        sum += number;
    }

    return sum;
}

void HellmanMerkleKnapsack::makePublicKey()
{
    public_key.reserve(sequence.size());
    for (const auto &value : sequence)
    {
        public_key.push_back((value * div) % mod);
    }
}

uint8_t HellmanMerkleKnapsack::makePlainByte(BigInteger value) const
{
    uint8_t byte = 0;
    uint8_t pos = 7;
    while (value > 0)
    {
        while (value < sequence[pos])
        {
            pos--;
        }
        byte = uint8::setBitAtPosition(byte, 7 - pos);
        value -= sequence[pos];
    }

    return byte;
}

BigIntVector HellmanMerkleKnapsack::encode(const BytesVector &message)
{
    const uint64_t message_len = message.size();
    BigIntVector crypted;
    crypted.reserve(message_len);

    for (const auto byte : message)
    {
        BigInteger sum;
        for (uint8_t i = 0; i < 8; ++i)
        {
            sum += static_cast<BigInteger> (uint8::getBitAtPosition(byte, 7 - i)) * public_key[i];
        }
        crypted.push_back(sum);
    }

    return crypted;
}

BytesVector HellmanMerkleKnapsack::decode(const BigIntVector &cipher)
{
    BytesVector decrypted;
    decrypted.reserve(cipher.size());

    const BigInteger inverse_div = Maths::getModInverse(div, mod);

    for (const auto &value : cipher)
    {
        decrypted.push_back(makePlainByte((value * inverse_div) % mod));
    }

    return decrypted;
}