#include "Camellia.hpp"

#include "exceptions/BadKeyLength.hpp"

using namespace CryptoGL;

constexpr std::array<uint64_t, 6> Camellia::key_sigma;
constexpr std::array<uint32_t, 256> Camellia::SP1110;
constexpr std::array<uint32_t, 256> Camellia::SP0222;
constexpr std::array<uint32_t, 256> Camellia::SP3033;
constexpr std::array<uint32_t, 256> Camellia::SP4404;

void Camellia::setKey(const BytesVector &key)
{
    const uint8_t key_size = key.size();
    if (key_size != 16 && key_size != 24 && key_size != 32)
    {
        throw BadKeyLength("Your key length has to be 16, 24 or 32 bytes.", key_size);
    }

    if (key_size == 16)
    {
        rounds = 18;
    }

    this->key = key;
}

void Camellia::generateSubkeys()
{
    BytesVector key_right, key_left;
    key_right.reserve(24);
    
    switch (key.size())
    {
        case 16:
            key_left = key;
            key_right = BytesVector(16, 0);//.insert(key_right.end(), 16, 0);
            Ke.reserve(4);
            break;

        case 24:
            key_left = key.range(0, 16);
            key_right = key.range(16);
            for (uint8_t i = 0; i < 8; ++i)
            { // NOT(x AND t) = NOT(x) AND t = x XOR t, where t is tautology.
                key_right.push_back(key_right[i] ^ 0xFF);
            }
            Ke.reserve(6);
            break;

        case 32:
            key_left = key.range(0, 16);
            key_right = key.range(16);
            Ke.reserve(6);
            break;
    }

    const BytesVector key_xored = key_left.Xor(key_right);
    uint64_t key_xored_left = BigEndian64::toIntegerRange(key_xored, 0, 8);
    uint64_t key_xored_right = BigEndian64::toIntegerRange(key_xored, 8) ^ F(key_xored_left ^ key_sigma[0], 0);
    
    key_xored_left ^= F(key_xored_right ^ key_sigma[1], 1)
                    ^ BigEndian64::toIntegerRange(key_left, 0, 8);
    key_xored_right ^= BigEndian64::toIntegerRange(key_left, 8)
                    ^ F(key_xored_left ^ key_sigma[2], 2);
    key_xored_left ^= F(key_xored_right ^ key_sigma[3], 3);
    
    BytesVector Ka = BigEndian64::toBytesVector(key_xored_left);
    Ka.extend(BigEndian64::toBytesVector(key_xored_right));

    processPrewhiteningPhase(Ka, key_left, key_right);
}

void Camellia::processPrewhiteningPhase(const BytesVector &Ka, const BytesVector &key_left, const BytesVector &key_right)
{
    Kw.extend(BigEndian64::toIntegersVector(key_left));
    subkeys.reserve(rounds);

    if (rounds == 18)
    {
        extendSubKeys18Rounds(Ka, key_left);
    }
    else
    {
        const BytesVector K = Ka.Xor(key_right);
        uint64_t K1 = BigEndian64::toIntegerRange(K, 0, 8);
        const uint64_t K2 = BigEndian64::toIntegerRange(K, 8) ^ F(K1 ^ key_sigma[4], 4);
        K1 ^= F(K2 ^ key_sigma[5], 5);

        BytesVector Kb = BigEndian64::toBytesVector(K1);
        Kb.extend(BigEndian64::toBytesVector(K2));

        extendSubKeysNot18Rounds(Ka, key_left, key_right, Kb);
    }
}

void Camellia::extendSubKeys18Rounds(const BytesVector &Ka, const BytesVector &key_left)
{
    subkeys.extend(BigEndian64::toIntegersVector(Ka));
    subkeys.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(15)));
    subkeys.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(15)));
    Ke.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(30)));
    subkeys.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(45)));

    subkeys.push_back(BigEndian64::toIntegerRange(Ka.rotateLeft(45), 0, 8));
    subkeys.push_back(BigEndian64::toIntegerRange(key_left.rotateLeft(60), 8));

    subkeys.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(60)));
    Ke.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(77)));
    subkeys.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(94)));
    subkeys.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(94)));
    subkeys.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(111)));
    Kw.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(111)));
}

void Camellia::extendSubKeysNot18Rounds(const BytesVector &Ka, const BytesVector &key_left,
                                        const BytesVector &key_right, const BytesVector &Kb)
{
    subkeys.extend(BigEndian64::toIntegersVector(Kb));
    subkeys.extend(BigEndian64::toIntegersVector(key_right.rotateLeft(15)));
    subkeys.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(15)));
    
    Ke.extend(BigEndian64::toIntegersVector(key_right.rotateLeft(30)));
    
    subkeys.extend(BigEndian64::toIntegersVector(Kb.rotateLeft(30)));
    subkeys.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(45)));
    subkeys.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(45)));
    
    Ke.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(60)));
    
    subkeys.extend(BigEndian64::toIntegersVector(key_right.rotateLeft(60)));
    subkeys.extend(BigEndian64::toIntegersVector(Kb.rotateLeft(60)));
    subkeys.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(77)));
    
    Ke.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(77)));
    
    subkeys.extend(BigEndian64::toIntegersVector(key_right.rotateLeft(94)));
    subkeys.extend(BigEndian64::toIntegersVector(Ka.rotateLeft(94)));
    subkeys.extend(BigEndian64::toIntegersVector(key_left.rotateLeft(111)));
    
    Kw.extend(BigEndian64::toIntegersVector(Kb.rotateLeft(111)));
}

uint64_t Camellia::F(const uint64_t half_block, const uint8_t) const
{
    uint32_t D = SP1110[half_block & 0xFF] 
               ^ SP0222[(half_block >> 24) & 0xFF]
               ^ SP3033[(half_block >> 16) & 0xFF] 
               ^ SP4404[(half_block >> 8) & 0xFF];

    uint32_t U = SP1110[(half_block >> 56) & 0xFF] 
               ^ SP0222[(half_block >> 48) & 0xFF]
               ^ SP3033[(half_block >> 40) & 0xFF] 
               ^ SP4404[(half_block >> 32) & 0xFF];

    D ^= U;
    U = D ^ uint32::rotateRight(U, 8);

    return (static_cast<uint64_t> (D) << 32) | U;
}

constexpr uint64_t Camellia::FL(const uint64_t &half_block, const uint64_t &subkey)
{
    const uint32_t Kl = subkey >> 32;
    const uint32_t Kr = subkey & 0xFFFFFFFF;
    
    uint32_t Xl = half_block >> 32;
    const uint32_t Xr = (half_block & 0xFFFFFFFF) ^ uint32::rotateLeft(Xl & Kl, 1);

    Xl ^= Xr | Kr;

    return (static_cast<uint64_t> (Xl) << 32) | Xr;
}

constexpr uint64_t Camellia::FLInverse(const uint64_t &half_block, const uint64_t &subkey)
{
    const uint32_t Kr = subkey & 0xFFFFFFFF;
    const uint32_t Kl = subkey >> 32;
    
    uint32_t Xr = half_block & 0xFFFFFFFF;
    const uint32_t Xl = (half_block >> 32) ^ (Xr | Kr);

    Xr ^= uint32::rotateLeft(Xl & Kl, 1);

    return (static_cast<uint64_t> (Xl) << 32) | Xr;
}

void Camellia::encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
    for (uint8_t j = 0; j < 6; j += 2)
    {
        R ^= F(L ^ subkeys[j], 0);
        L ^= F(R ^ subkeys[j + 1], 0);
    }

    const uint8_t big_rounds = rounds / 4;
    for (uint8_t i = 0; i < big_rounds; i += 2)
    {
        L = FL(L, Ke[i]);
        R = FLInverse(R, Ke[i + 1]);

        const uint8_t index = 3 * i;
        for (uint8_t j = 6; j < 12; j += 2)
        {
            R ^= F(L ^ subkeys[index + j], 0);
            L ^= F(R ^ subkeys[index + j + 1], 0);
        }
    }

    R ^= Kw[2];
    L ^= Kw[3];
}

void Camellia::decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const
{
    const uint8_t lower_bound = subkeys.size() - 6;
    for (uint8_t i = subkeys.size() - 1; i >= lower_bound; i -= 2)
    {
        R ^= F(L ^ subkeys[i], 0);
        L ^= F(R ^ subkeys[i - 1], 0);
    }

    const uint8_t big_rounds = rounds / 8;
    for (int8_t i = big_rounds - 1; i >= 0; --i)
    {
        const uint8_t Ke_index = 2 * i;
        L = FL(L, Ke[Ke_index + 1]);
        R = FLInverse(R, Ke[Ke_index]);

        const uint8_t index = 6 * i;
        for (int8_t j = 5; j >= 0; j -= 2)
        {
            R ^= F(L ^ subkeys[index + j], 0);
            L ^= F(R ^ subkeys[index + j - 1], 0);
        }
    }
    R ^= Kw[0];
    L ^= Kw[1];
}

void Camellia::processEncodingCurrentBlock()
{
    uint64_t L0 = current_block[0] ^ Kw[0];
    uint64_t R0 = current_block[1] ^ Kw[1];
    encodeFeistelRounds(L0, R0, 0);

    current_block = {R0, L0};
}

void Camellia::processDecodingCurrentBlock()
{
    uint64_t L0 = current_block[0] ^ Kw[2];
    uint64_t R0 = current_block[1] ^ Kw[3];
    decodeFeistelRounds(L0, R0, 0);

    current_block = {R0, L0};
}