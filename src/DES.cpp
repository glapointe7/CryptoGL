#include "DES.hpp"

#include "Bits.hpp"

#include "exceptions/BadKeyLength.hpp"
#include "exceptions/EmptyKey.hpp"

using namespace CryptoGL;

constexpr std::array<uint8_t, 16> DES::rot_table;
constexpr std::array<uint8_t, 64> DES::IP;
constexpr std::array<uint8_t, 64> DES::IP_inverse;
constexpr std::array<uint8_t, 48> DES::E;
constexpr std::array<uint8_t, 32> DES::P;
constexpr std::array<std::array<std::array<uint8_t, 16>, 4>, 8> DES::S;
constexpr std::array<uint8_t, 56> DES::PC1;
constexpr std::array<uint8_t, 48> DES::PC2;

void DES::setKey(const BytesVector &key)
{
    if (key.size() != 8)
    {
        throw BadKeyLength("The key must be 8 bytes length.", key.size());
    }

    this->key = key;
}

void DES::generateSubkeys()
{
    // We permute with the PC1 table to get a 56-bits key.
    const uint64_t K56 = getBitsFromTable<56>(BigEndian64::toInteger(key), PC1, 64);

    // Split the 56-bits key in 2 28-bits sub-keys.
    uint64_t K1 = K56 & 0xFFFFFFF;
    uint64_t K2 = (K56 & 0xFFFFFFF0000000) >> 28;

    subkeys.reserve(16);
    for (uint8_t i = 0; i < 16; ++i)
    {
        const uint64_t KR = Bits::rotateLeft(K1, rot_table[i], 28);
        const uint64_t KL = Bits::rotateLeft(K2, rot_table[i], 28);
        K1 = KR;
        K2 = KL;

        const uint64_t K = KR | (KL << 28);
        subkeys.push_back(getBitsFromTable<48>(K, PC2, 56));
    }
}

uint64_t DES::getSubstitution(const uint64_t &key_mixed)
{
    std::array<uint8_t, 8> sboxes;
    for (uint8_t i = 0, j = 42; i < 8; ++i, j -= 6)
    {
        sboxes[i] = (key_mixed >> j)  % 64;
    }

    uint64_t s_block = 0;
    for (uint8_t i = 0, j = 28; i < 8; ++i, j -= 4)
    {
        const uint64_t row = (sboxes[i] % 2) | ((sboxes[i] >> 5) << 1);
        const uint64_t col = (sboxes[i] >> 1) % 16;
        s_block |= (S[i][row][col] << j);
    }

    return s_block;
}

uint64_t DES::F(const uint64_t data, const uint8_t round) const
{
    // Expension from 32 bits to 48 bits.
    const uint64_t E_block = getBitsFromTable<48>(data, E, 32);

    const uint64_t key_mixed = E_block ^ subkeys[round];

    // Substitutions with the 8 S-Boxes. Output of 32 bits.
    const uint64_t s_block = getSubstitution(key_mixed);

    return getBitsFromTable<32>(s_block, P, 32);
}

void DES::encodeFeistelRounds(uint64_t& L, uint64_t& R, const uint8_t) const
{
    for (uint8_t i = 0; i < rounds; ++i)
    {
        const uint64_t tmp = L ^ F(R, i);
        L = R;
        R = tmp;
    }
}

void DES::decodeFeistelRounds(uint64_t& L, uint64_t& R, const uint8_t) const
{
    for (int8_t i = rounds - 1; i >= 0; --i)
    {
        const uint64_t tmp = L ^ F(R, i);
        L = R;
        R = tmp;
    }
}

void DES::processEncodingCurrentBlock()
{
    // Initial permutation of the 64-bits data blocks.
    const uint64_t ip_data = getBitsFromTable<64>(current_block, IP, 64);

    // Split the 64-bits block in 2 blocks of 32 bits L and R for the 16 Feistel rounds.
    uint64_t L = ip_data >> 32;
    uint64_t R = ip_data & 0xFFFFFFFF;
    encodeFeistelRounds(L, R, 0);

    // Final permutation of R16.L16. with the IP_inverse table.
    const uint64_t RL = (R << 32) | L;
    current_block = getBitsFromTable<64>(RL, IP_inverse, 64);
}

void DES::processDecodingCurrentBlock()
{
    // Initial permutation of the 64-bits data blocks.
    const uint64_t ip_data = getBitsFromTable<64>(current_block, IP, 64);

    // Split the 64-bits block in 2 blocks of 32 bits L and R for the 16 Feistel rounds.
    uint64_t L = ip_data >> 32;
    uint64_t R = ip_data & 0xFFFFFFFF;
    decodeFeistelRounds(L, R, 0);

    // Final permutation of R16.L16. with the IP_inverse table.
    const uint64_t RL = (R << 32) | L;
    current_block = getBitsFromTable<64>(RL, IP_inverse, 64);
}