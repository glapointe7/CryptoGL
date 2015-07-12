#include "Snow3G.hpp"

#include "exceptions/BadKeyLength.hpp"

using namespace CryptoGL;

constexpr std::array<uint8_t, 256> Snow3G::SQ;
constexpr std::array<uint8_t, 256> Snow3G::SR;

void Snow3G::setIV(const BytesVector &IV)
{
    const uint8_t iv_size = IV.size();
    if (iv_size != 16)
    {
        throw BadIVLength("Your IV has to be 128 bits length.", iv_size);
    }
    this->IV = IV;
}

constexpr uint8_t Snow3G::mulx(const uint8_t V, const uint8_t c)
{
    return V & 0x80 ? (V * 2) ^ c : V * 2;
}

constexpr uint8_t Snow3G::mulxPow(const uint8_t V, const uint8_t i, const uint8_t c)
{
    if (i == 0)
        return V;

    return mulx(mulxPow(V, i - 1, c), c);
}

uint32_t Snow3G::mulAlpha(const uint8_t c)
{
    return BigEndian32::toInteger({
        mulxPow(c, 23, 0xA9), 
        mulxPow(c, 245, 0xA9),
        mulxPow(c, 48, 0xA9), 
        mulxPow(c, 239, 0xA9)
    });
}

uint32_t Snow3G::divAlpha(const uint8_t c)
{
    return BigEndian32::toInteger({
        mulxPow(c, 16, 0xA9), 
        mulxPow(c, 39, 0xA9),
        mulxPow(c, 6, 0xA9), 
        mulxPow(c, 64, 0xA9)
    });
}

void Snow3G::nextState(const uint32_t v)
{
    for (uint8_t i = 0; i < 15; ++i)
    {
        state[i] = state[i + 1];
    }
    state[15] = v;
}

uint32_t Snow3G::getv() const
{
    return ((state[0] << 8) & 0xFFFFFF00) 
         ^ mulAlpha(state[0] >> 24) 
         ^ state[2]
         ^ ((state[11] >> 8) & 0x00FFFFFF) 
         ^ divAlpha(state[11] & 0xFF);
}

void Snow3G::initializationMode(const uint32_t F)
{
    nextState(getv() ^ F);
}

void Snow3G::keystreamMode()
{
    nextState(getv());
}

uint32_t Snow3G::S(const uint32_t w, const std::array<uint8_t, 256> &sbox, const uint8_t c)
{
    const std::array<uint8_t, 4> SBox = {{
         sbox[w >> 24], 
         sbox[(w >> 16) & 0xFF],
         sbox[(w >> 8) & 0xFF], 
         sbox[w & 0xFF]
    }};

    const BytesVector r = {
        static_cast<uint8_t>(mulx(SBox[0], c) ^ SBox[1] ^ SBox[2] ^ mulx(SBox[3], c) ^ SBox[3]),
        static_cast<uint8_t>(mulx(SBox[0], c) ^ SBox[0] ^ mulx(SBox[1], c) ^ SBox[2] ^ SBox[3]),
        static_cast<uint8_t>(SBox[0] ^ mulx(SBox[1], c) ^ SBox[1] ^ mulx(SBox[2], c) ^ SBox[3]),
        static_cast<uint8_t>(SBox[0] ^ SBox[1] ^ mulx(SBox[2], c) ^ SBox[2] ^ mulx(SBox[3], c))
    };

    return BigEndian32::toInteger(r);
}

uint32_t Snow3G::S1(const uint32_t w)
{
    return S(w, SR, 0x1B);
}

uint32_t Snow3G::S2(const uint32_t w)
{
    return S(w, SQ, 0x69);
}

uint32_t Snow3G::finiteStateMachine()
{
    const uint32_t F = (state[15] + R1) ^ R2;
    const uint32_t r = R2 + (R3 ^ state[5]);
    R3 = S2(R2);
    R2 = S1(R1);
    R1 = r;

    return F;
}

void Snow3G::keySetup()
{
    const UInt32Vector K = BigEndian32::toIntegersVector(key);
    const UInt32Vector Iv = BigEndian32::toIntegersVector(IV);

    constexpr uint32_t MAX_UINT32_CONSTANT = std::numeric_limits<uint32_t>::max();
    state.resize(16);
    
    state[15] = K[3] ^ Iv[0];
    state[14] = K[2];
    state[13] = K[1];
    state[12] = K[0] ^ Iv[1];
    
    state[11] = K[3] ^ MAX_UINT32_CONSTANT;
    state[10] = K[2] ^ MAX_UINT32_CONSTANT ^ Iv[2];
    state[9] = K[1] ^ MAX_UINT32_CONSTANT ^ Iv[3];
    state[8] = K[0] ^ MAX_UINT32_CONSTANT;
    
    state[7] = K[3];
    state[6] = K[2];
    state[5] = K[1];
    state[4] = K[0];
    
    state[3] = K[3] ^ MAX_UINT32_CONSTANT;
    state[2] = K[2] ^ MAX_UINT32_CONSTANT;
    state[1] = K[1] ^ MAX_UINT32_CONSTANT;
    state[0] = K[0] ^ MAX_UINT32_CONSTANT;

    R1 = R2 = R3 = 0;

    // The FSM is clocked producing the 32-bit words F.
    // Then, the LFSR is clocked in Initialization Mode consuming F.
    for (uint8_t i = 0; i < 32; ++i)
    {
        const uint32_t F = finiteStateMachine();
        initializationMode(F);
    }
}

UInt32Vector Snow3G::generateKeystream()
{
    finiteStateMachine();
    keystreamMode();

    UInt32Vector keystream;
    keystream.reserve(output_size);
    for (uint32_t t = 0; t < output_size; ++t)
    {
        const uint32_t F = finiteStateMachine();
        keystream.push_back(F ^ state[0]);
        keystreamMode();
    }

    return keystream;
}