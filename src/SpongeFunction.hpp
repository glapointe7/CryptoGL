/*
 * Source : http://sponge.noekeon.org/CSF-0.1.pdf
 */
#ifndef SPONGEFUNCTION_HPP
#define SPONGEFUNCTION_HPP

#include <vector>
#include <stdint.h>

template <class UInt>
class SpongeFunction
{
protected:
   typedef std::vector<uint8_t> BytesContainer;
   typedef std::vector<uint64_t> UInt64Container;
   typedef std::vector<UInt> UIntContainer;
   typedef std::vector<UIntContainer> UIntMatrix;
   typedef std::vector<std::vector<uint64_t> > Matrix;

   /* Default constructor : b = 1600 => r = 1024 et c = 576. */
   SpongeFunction()
   : bitrate(1024), capacity(576), width(1600), lane_width(64), block_size(128), output_size(512), rounds(24) {}

   /* Constructor with output size, capacity and bitrate given as parameters. */
   SpongeFunction(const uint16_t hash_size, const uint16_t c, const uint16_t r, const uint8_t round)
   : bitrate(r), capacity(c), width(r + c), lane_width((r + c) / 25), 
     block_size(200 * r / (r + c)), output_size(hash_size), rounds(round) {}

   virtual ~SpongeFunction() {}

   virtual void applyAbsorbingPhase(const BytesContainer &padded_message) = 0;
   virtual const BytesContainer applySqueezingPhase() = 0;
   virtual void F() = 0;

   /* Apply Pad10* rule from the specs. 1 bit upto block_size bits are appended. */
   const BytesContainer applyPad10Star(const BytesContainer &message) const
   {
      const uint64_t message_len = message.size();
      const uint8_t block_size = bitrate >> 3;

      /* Append '1' bit to the message. */
      BytesContainer padding(message);
      padding.reserve(message_len + block_size);
      padding.push_back(0x01);

      /* Append enough '0' bits such that the length will be a multiple of 'block_size'. */
      const uint8_t bytes_to_append = block_size - ((message_len + 1) % block_size);
      if (bytes_to_append != block_size)
      {
         padding.insert(padding.end(), bytes_to_append, 0);
      }

      return padding;
   }

   /* Apply padding Pad10*1 rule from the specs. 2 bits upto block_size + 1 bits are appended. */
   const BytesContainer applyPad10Star1(const BytesContainer &message) const
   {
      const uint8_t block_size = bitrate >> 3;
      BytesContainer padding(message);
      padding.reserve(message.size() + block_size + 1);

      if(!((message.size() + 1) % block_size))
      {
         padding.push_back(0x81);
      }
      else
      {
         padding.push_back(0x01);
         padding.insert(padding.end(), block_size - (padding.size() % block_size), 0);
         padding.back() = 0x80;
      }

      return padding;
   }

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
   UIntMatrix state;
   
public:

   /* General encoding using the sponge construction. */
   virtual const BytesContainer encode(const BytesContainer &data) final
   {
      state.resize(5);
      for(uint8_t x = 0; x < 5; ++x)
      {
         state[x].resize(5);
      }
 
      const BytesContainer padded_message = applyPad10Star1(data);
      
      applyAbsorbingPhase(padded_message);

      return applySqueezingPhase();
   }
};

#endif