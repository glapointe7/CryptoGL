#include "CAST256.hpp"

#include "Bits.hpp"
#include "Padding.hpp"

using namespace CryptoGL;

constexpr std::array<std::array<uint32_t, 256>, 4> CAST256::S;
const std::array<Function, 3> CAST256::F = {{F1, F2, F3}};

void CAST256::setKey(const BytesVector &key)
{
    const uint8_t keylen = key.size();
    constexpr std::array<uint8_t, 5> allowed_key_lengths = {{16, 20, 24, 28, 32}};

    const auto it = std::find(allowed_key_lengths.begin(), allowed_key_lengths.end(), keylen); 
    if (it == allowed_key_lengths.end())
    {
        throw BadKeyLength("Your key length has to be of length 16, 20, 24, 28 or 32 bytes.", keylen);
    }

    // Pad the key with 0 to get 256 bits length.
    this->key = Padding::zeros(key, 32);
}

void CAST256::applyForwardQuadRound(const uint8_t round)
{
    const uint8_t j = round * 4;
    current_block[2] ^= F[0](current_block[3], subkeys[j], Kr[j]);
    current_block[1] ^= F[1](current_block[2], subkeys[j + 1], Kr[j + 1]);
    current_block[0] ^= F[2](current_block[1], subkeys[j + 2], Kr[j + 2]);
    current_block[3] ^= F[0](current_block[0], subkeys[j + 3], Kr[j + 3]);
}

void CAST256::applyReverseQuadRound(const uint8_t round)
{
    const uint8_t j = round * 4;
    current_block[3] ^= F[0](current_block[0], subkeys[j + 3], Kr[j + 3]);
    current_block[0] ^= F[2](current_block[1], subkeys[j + 2], Kr[j + 2]);
    current_block[1] ^= F[1](current_block[2], subkeys[j + 1], Kr[j + 1]);
    current_block[2] ^= F[0](current_block[3], subkeys[j], Kr[j]);
}

void CAST256::applyForwardOctave(UInt32Vector &kappa, const uint8_t round) const
{
    const uint8_t j = round * 8;
    for (uint8_t i = 0; i < 8; ++i)
    {
        kappa[(14 - i) % 8] ^= F[i % 3](kappa[7 - i], Tm[j + i], Tr[j + i]);
    }
}

void CAST256::generateSubkeys()
{
    Tm.reserve(192);
    Tr.reserve(192);

    constexpr uint32_t Mm = 0x6ED9EBA1;
    constexpr uint8_t Mr = 17;
    uint32_t Cm = 0x5A827999;
    uint8_t Cr = 19;
    for (uint8_t i = 0; i < 192; ++i)
    {
        Tm.push_back(Cm);
        Cm += Mm;
        Tr.push_back(Cr);
        Cr = (Cr + Mr) % 32;
    }

    subkeys.reserve(48);
    Kr.reserve(48);
    UInt32Vector kappa = BigEndian32::toIntegersVector(key);
    for (uint8_t i = 0; i < rounds; ++i)
    {
        const uint8_t k = 2 * i;
        applyForwardOctave(kappa, k);
        applyForwardOctave(kappa, k + 1);
        for (uint8_t j = 0; j < 8; j += 2)
        {
            Kr.push_back(kappa[j] % 32);
            subkeys.push_back(kappa[7 - j]);
        }
    }
}

constexpr uint32_t CAST256::F1(const uint32_t D, const uint32_t Km, const uint32_t Kr)
{
    const uint32_t I = Bits::rotateLeft(Km + D, Kr);
    
    return ((S[0][getByteFromInteger<3>(I)] ^ S[1][getByteFromInteger<2>(I)])
           - S[2][getByteFromInteger<1>(I)]) + S[3][getByteFromInteger<0>(I)];
}

constexpr uint32_t CAST256::F2(const uint32_t D, const uint32_t Km, const uint32_t Kr)
{
    const uint32_t I = Bits::rotateLeft(Km ^ D, Kr);
    
    return ((S[0][getByteFromInteger<3>(I)] - S[1][getByteFromInteger<2>(I)])
           + S[2][getByteFromInteger<1>(I)]) ^ S[3][getByteFromInteger<0>(I)];
}

constexpr uint32_t CAST256::F3(const uint32_t D, const uint32_t Km, const uint32_t Kr)
{
    const uint32_t I = Bits::rotateLeft(Km - D, Kr);
    
    return ((S[0][getByteFromInteger<3>(I)] + S[1][getByteFromInteger<2>(I)])
           ^ S[2][getByteFromInteger<1>(I)]) - S[3][getByteFromInteger<0>(I)];
}

void CAST256::processEncodingCurrentBlock()
{
    for (uint8_t i = 0; i < 6; ++i)
    {
        applyForwardQuadRound(i);
    }
    
    for (uint8_t i = 6; i < rounds; ++i)
    {
        applyReverseQuadRound(i);
    }
}

void CAST256::processDecodingCurrentBlock()
{
    for (uint8_t i = rounds - 1; i >= 6; --i)
    {
        applyForwardQuadRound(i);
    }
    
    for (int8_t i = 5; i >= 0; --i)
    {
        applyReverseQuadRound(i);
    }
}