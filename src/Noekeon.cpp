#include "Noekeon.hpp"

#include <algorithm>

#include "Bits.hpp"

#include "exceptions/BadKeyLength.hpp"

using namespace CryptoGL;

constexpr std::array<uint32_t, 17> Noekeon::round_constants;

void Noekeon::generateSubkeys()
{
    subkeys = BigEndian32::toIntegersVector(key);
}

void Noekeon::generateInverseSubkeys()
{
    generateSubkeys();
    applyThetaToSubkeys();
}

void Noekeon::applyGamma()
{
    current_block[1] ^= ~(current_block[3] | current_block[2]);
    current_block[0] ^= current_block[2] & current_block[1];

    std::swap(current_block[3], current_block[0]);
    current_block[2] ^= current_block[0] ^ current_block[1] ^ current_block[3];

    current_block[1] ^= ~(current_block[3] | current_block[2]);
    current_block[0] ^= current_block[2] & current_block[1];
}

void Noekeon::setClassVector(const std::array<uint8_t, 4> &indices, UInt32Vector &V)
{
    uint32_t temp = V[indices[0]] ^ V[indices[1]];
    temp ^= Bits::rotateRight(temp, 8) ^ Bits::rotateLeft(temp, 8);
    V[indices[2]] ^= temp;
    V[indices[3]] ^= temp;
}

void Noekeon::applyThetaToSubkeys()
{
    setClassVector({{0, 2, 1, 3}}, subkeys);
    setClassVector({{1, 3, 0, 2}}, subkeys);
}

void Noekeon::applyThetaToState()
{
    setClassVector({{0, 2, 1, 3}}, current_block);

    for (uint8_t i = 0; i < 4; ++i)
    {
        current_block[i] ^= subkeys[i];
    }

    setClassVector({{1, 3, 0, 2}}, current_block);
}

void Noekeon::applyPiLeft()
{
    current_block[1] = Bits::rotateLeft(current_block[1], 1);
    current_block[2] = Bits::rotateLeft(current_block[2], 5);
    current_block[3] = Bits::rotateLeft(current_block[3], 2);
}

void Noekeon::applyPiRight()
{
    current_block[1] = Bits::rotateRight(current_block[1], 1);
    current_block[2] = Bits::rotateRight(current_block[2], 5);
    current_block[3] = Bits::rotateRight(current_block[3], 2);
}

void Noekeon::applyRound(const uint8_t constant1, const uint8_t constant2)
{
    current_block[0] ^= constant1;
    applyThetaToState();
    current_block[0] ^= constant2;
    applyPiLeft();
    applyGamma();
    applyPiRight();
}

void Noekeon::processEncodingCurrentBlock()
{
    for (uint8_t i = 0; i < rounds; ++i)
    {
        applyRound(round_constants[i], 0);
    }
    current_block[0] ^= round_constants[rounds];
    applyThetaToState();

}

void Noekeon::processDecodingCurrentBlock()
{
    for (uint8_t i = rounds; i > 0; --i)
    {
        applyRound(0, round_constants[i]);
    }
    applyThetaToState();
    current_block[0] ^= round_constants[0];

}