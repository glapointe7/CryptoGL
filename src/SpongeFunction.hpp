/*
 * Source : http://sponge.noekeon.org/CSF-0.1.pdf
 */
#ifndef SPONGEFUNCTION_HPP
#define SPONGEFUNCTION_HPP

#include <vector>

#include "Types.hpp"
#include "Padding.hpp"

namespace CryptoGL
{
    template <typename DataType>
    class SpongeFunction
    {
    protected:
       using DataTypeVector = std::vector<DataType>;
       using DataTypeMatrix = std::vector<DataTypeVector>;

       /* Constructor with output size, capacity and bitrate given as parameters. */
       SpongeFunction(const uint16_t hash_size, const uint16_t c, const uint16_t r, const uint8_t round)
          : bitrate(r), capacity(c), width(r + c), lane_width((r + c) / 25), 
             block_size(200 * r / (r + c)), output_size(hash_size), rounds(round) {}

       /* Default constructor : b = 1600 => r = 1024 et c = 576. */
       SpongeFunction() : SpongeFunction(512, 576, 1024, 24) {}

       virtual ~SpongeFunction() {}

       virtual void applyAbsorbingPhase(const BytesVector &padded_message) = 0;
       virtual BytesVector applySqueezingPhase() = 0;
       virtual void F() = 0;

       /* Bitrate (or r in the spec.) : default = 1024. */
       const uint16_t bitrate;

       /* Capacity (or c in the spec.) : default = 576 bits. */
       const uint16_t capacity;

       /* width (or b in the spec.) : {25, 50, 100, 200, 400, 800, default = 1600}. */
       const uint16_t width;

       /* Calculated using width / 25. Default = 64. */
       const uint8_t lane_width;

       /* Block size is calculated using bitrate / lane_width * 8. */
       const uint8_t block_size;

       /* Possible output sizes are {224, 256, 384, 512} in bits. */
       const uint16_t output_size;

       /* Number of rounds following this formula : 2 * log_2(b / 25) + 12. */
       const uint8_t rounds;

       /* The state of the sponge construction. */
       DataTypeMatrix state;

    public:
       /* General encoding using the sponge construction. */
       BytesVector encode(BytesVector data)
       {
          state.resize(5);
          for(uint8_t x = 0; x < 5; ++x)
          {
             state[x].resize(5);
          }

          data = Padding::_10Star1(data, bitrate / 8);

          applyAbsorbingPhase(data);

          return applySqueezingPhase();
       }
    };
}

#endif