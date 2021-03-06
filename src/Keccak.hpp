/*
 * Source : http://keccak.noekeon.org/Keccak-implementation-3.2.pdf
 */
#ifndef KECCAK_HPP
#define KECCAK_HPP

#include "SpongeFunction.hpp"

#include <array>

namespace CryptoGL
{
    class Keccak : public SpongeFunction<uint64_t>
    {
    public:
        /* Default constructor : output size of 512 bits, with capacity of 576 bits 
           bitrate of 1024 bits and 24 rounds. */
        Keccak() : SpongeFunction() { }

        /* Predefined hash sizes for width = 1600. */
        enum class HashSize : uint16_t
        {
            _224bits = 224,
            _256bits = 256,
            _384bits = 384,
            _512bits = 512
        };
        
        explicit Keccak(const HashSize hash_size)
            : SpongeFunction(static_cast<uint16_t> (hash_size),
                static_cast<uint16_t> (hash_size) << 1, 
                1600 - (static_cast<uint16_t> (hash_size) << 1), 24) { }

    private:
        /* Keccak-f permutation function. */
        void F() override;

        /* Apply the absorbing phase from the sponge construction using the padded input message.*/
        void applyAbsorbingPhase(const BytesVector &padded_message) override;

        /* Apply the squeezing phase from the sponge construction and return the final output hash. */
        BytesVector applySqueezingPhase() override;

        /* Apply theta, rho, pi, chi and iota steps from the specs. */
        void applyRound(const uint8_t round_index);

        /* Round constants. */
        static constexpr std::array<uint64_t, 24> round_constants = {{
            0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
            0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009,
            0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
            0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
            0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
            0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
        }};

        /* For the rho step. */
        static constexpr std::array<std::array<uint8_t, 5>, 5> rho_offsets = {{
            {{0, 36, 3, 41, 18}},
            {{1, 44, 10, 45, 2}},
            {{62, 6, 43, 15, 61}},
            {{28, 55, 25, 21, 56}},
            {{27, 20, 39, 8, 14}}
        }};
    };
}

#endif